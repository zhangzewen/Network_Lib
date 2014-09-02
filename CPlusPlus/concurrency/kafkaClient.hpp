#ifndef _YISIS_KAFKACLIENT_HPP_INCLUDED_
#define _YISIS_KAFKACLIENT_HPP_INCLUDED_

class kafkaClient
{
public:
	typedef enum {
		PRODUCTOR,
		CONSUMER
	}CLIENTTYPE;
	kafkaClient(CLIENTTYPE type) : type_(type) {}
	~kafkaClient() {}
	void SetTopic(const std::string& topic);
	void SetBrokers(const std::string& brokers);
private:
	CLIENTTYPE type_;
	std::string topic_;
	std::string brokers_;
};

#endif
