#include "MessageTransport.h"

#include "sp/Json.h"

#include "ext/json_input.h"
#include "ext/json_output.h"

sf::Box<sv::Message> readMessage(bqws_msg *wsMsg)
{
	// sf::debugPrintJson("Receive", sf::String((char*)wsMsg->data, wsMsg->size));

	sf::Box<sv::Message> msg;
	jsi_args args = { };
	args.dialect.allow_comments = true;
	jsi_value *value = jsi_parse_memory(wsMsg->data, wsMsg->size, &args);
	bqws_free_msg(wsMsg);


	if (!sp::readJson(value, msg)) return { };
	jsi_free(value);
	return msg;
}

void writeMessage(bqws_socket *ws, sv::Message *msg, const sf::Symbol &from, const sf::Symbol &to)
{
	jso_stream s;
	jso_init_growable(&s);
	s.pretty = true;
	s.pretty_wrap = 80;

	sp::writeJson(s, msg);

	// sf::debugPrintJson("Send", sf::String((char*)s.data, s.pos));
	bqws_send_binary(ws, s.data, s.pos);
}
