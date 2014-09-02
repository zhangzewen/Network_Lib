#include "kafkaClient.hpp"

class ExampleDeliveryReportCb : public RdKafka::DeliveryReportCb {
public:
	void dr_cb(RdKafka::Message& message) {
		std::cout << "Message delivery for (" << message.len() << "bytes); " << message.errstr() << std::endl;
	}
};

class ExampleEventCb : public RdKafka::EventCb {
public:
	void event_cb(RdKafka::Event& event) {
		switch(event.type()) {
			case RdKafka::Event::EVENT_ERROR:
				std::cerr << "ERROR " << RdKafka::err2str(event.err()) << "):" << event.str() << std::endl;
				if (event.err() == RdKafka::ERR_ALL_BROKERS_DOWN)
					run = false;
				break;
			case RdKafka::Event::EVENT_STATS:
				std::cerr << "\"STATS\": " << event.str() << std::endl;
				break;
			case RdKafka::Event::EVENT_LOG:
				fprintf(stderr, "LOG-%i-%s: %s\n", event.severity(), event.fac().c_str(),
						event.str().c_str());
				break;
			default:
				std::cerr << "EVENT " << event.type() << " (" << RdKafka::err2str(event.err()) << "): " << event.str() << std::endl;
				break;
		}
	}
};


class MyHashPartitionerCb : public RdKafka::PartitionerCb {
public:
	int32_t partitioner_cb(const RdKafka::Topic *topic, const std::string* key,
			int32_t partition_cnt, void* msg_opaque) {
		return djb_hash(key->c_str(), key->size()) % partition_cnt;
	}
private:
	static inline unsigned int djb_hash (const char* str, size_t len) {
		unsigned int hash = 5381;
		for (size_t i = 0; i < len; i++) {
			hash = ((hash << 5) + hash) + str[i];
		}
		return hash;
	}
};


int main (int argc, char **argv)
{
	std::string brokers = "localhost";
	std::string errstr;
	std::string topic_str;
	std::string mode;
	std::string debug;

	int32_t partition = RdKafka::Topic::PRARTITION_UA;
	int64_t start_offset = RdKafka::Topic::OFFSET_BEGINNING;
	bool exit_eof = false;
	bool do_conf_dump = false;

	MyHashPartitionerCb hash_partitioner;

	RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
	RdKafka::Conf* tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);

	conf->set("metadata.broker.list", brokers, errstr);
	ExampleEventCb ex_event_cb;
	conf->set("event_cb", &ex_event_cb, errstr);

}
