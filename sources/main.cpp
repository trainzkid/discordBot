#include <dpp/dpp.h>
#include <dpp/cluster.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

void ping(const dpp::slashcommand_t& event) {
	if(event.command.get_command_name() == "ping") {
		event.reply("Pong!");
		std::clog<<"Received /ping, responded with Pong!"<<std::endl;
	}
}

std::string getBotToken() {
	std::ifstream file{"bot_token"};
	std::string BOT_TOKEN; 
	if(!file.is_open()) {
		std::cerr<<"Couldn't open bot token file!"<<std::endl;
		throw;
	} else {
		if(!(file>>BOT_TOKEN)) {
			std::cerr<<"Token couldn't be retreived from file!"<<std::endl;
			throw;
		}
		return BOT_TOKEN;
	}
}

int main() {
	dpp::cluster bot(getBotToken());

	bot.on_log(dpp::utility::cout_logger());

	bot.on_slashcommand(ping);

	auto registerPing = [&bot](const dpp::ready_t& event){
		if(dpp::run_once<struct register_bot_commands>()) {
			bot.global_command_create(
				dpp::slashcommand("ping", "Test server status", bot.me.id)
			);
		}
	};

	bot.on_ready(registerPing);

	bot.start(dpp::st_wait);

	return 0;
}
