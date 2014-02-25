#include "hiconfig.h"
#include <ThriftBroker/gen-cpp/Client_types.h>

#include "Dict.h"

Dict::Dict(htConnPoolPtr _conn_pool,
		const std::string _ns,
		const std::string _word_table,
		const std::string _stem_table,
		boost::function<uint64_t()> _genWordId,
		const LangDetect::Lang &_lang,
		Reset _reset):
	m_conn_pool(_conn_pool),
	m_ns(_ns),
	m_word_table(_word_table),
	m_stem_table(_stem_table),
	DictOffline(_lang, _genWordId)
{
	if (_reset == Dict::RESET) {
		htConnPool::htSession sess = _conn_pool->get();
		Hypertable::ThriftGen::Namespace ns = sess.client->namespace_open(_ns);
		Hypertable::ThriftGen::HqlResult result;
		sess.client->hql_query(result, ns, "drop table if exists "+_word_table);
		sess.client->hql_query(result, ns, "create table "+_word_table+\
			" (id MAX_VERSIONS=1)");
		sess.client->hql_query(result, ns, "drop table if exists "+_stem_table);
		sess.client->hql_query(result, ns, "create table "+_stem_table+\
			" (id MAX_VERSIONS=1)");
	}
	
	//std::cout << "Dict::Dict setting accessors\n";
	m_word_querier.reset(new htQuerier(_conn_pool, _ns, _word_table));
	m_stem_querier.reset(new htQuerier(_conn_pool, _ns, _stem_table));
	
	//std::cout << "Dict::Dict queuers set\n";
	
	m_word_writer.reset(new htCollWriterConc(_conn_pool, _ns, _word_table));
	m_stem_writer.reset(new htCollWriterConc(_conn_pool, _ns, _stem_table));
	
	//std::cout << "Dict::Dict writers set\n";
	
	htCollScanner word_scanner(_conn_pool, _ns, _word_table, "id");
	htCollScanner stem_scanner(_conn_pool, _ns, _stem_table, "id");
	
	//std::cout << "Dict::Dict accessors set\n";
	
	while (!word_scanner.end()) {
		KeyValue kv = word_scanner.getNextCell();
		uint64_t id;
		sscanf(kv.value.c_str(), "%llu", &id);
		m_word_ids.insert(std::pair<std::string, uint64_t>(kv.key, id));
		m_id_words.insert(std::pair<uint64_t, std::string>(id, kv.key));
	}
	while (!stem_scanner.end()) {
		KeyValue kv = stem_scanner.getNextCell();
		uint64_t id;
		sscanf(kv.value.c_str(), "%llu", &id);
		m_stem_ids.insert(std::pair<std::string, uint64_t>(kv.key, id));
	}
	
}

uint64_t Dict::getWordId(const std::string &_word)
{	
	uint64_t id = DictOffline::getWordId(_word);
	
	if (id != 0)
		return id;
	
	id = genWordId();
	
	std::string word = _word;
	fix_utf8_string(word);
	toLowerUtf8(word);
	
	char id_bf[50];
	sprintf(id_bf, "%llu", id);
	m_word_writer->insertSync(KeyValue(word, std::string(id_bf)), "id");
	
	m_word_ids.insert(std::pair<std::string, uint64_t>(word, id));
	m_id_words.insert(std::pair<uint64_t, std::string>(id, word));
}

uint64_t Dict::getStemId(const std::string &_word)
{
	uint64_t id = DictOffline::getStemId(_word);
	
	if (id != 0)
		return id;
	
	id = genWordId();
	
	std::string stem = _word;
	fix_utf8_string(stem);
	toLowerUtf8(stem);
	m_stemmer.stem(stem, stem);
	
	char id_bf[50];
	sprintf(id_bf, "%llu", id);
	m_stem_writer->insertSync(KeyValue(stem, std::string(id_bf)), "id");
	
	m_stem_ids.insert(std::pair<std::string, uint64_t>(stem, id));
	return id;
}

Dict::~Dict()
{

}
