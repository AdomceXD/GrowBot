#include "Database.hpp"
#include "dpp/dpp.h"
#include <iostream>

int main() {
    const string BOT_TOKEN = "Insert token ere'";

    Database db("data.json");
    dpp::cluster bot(BOT_TOKEN);
    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand([&bot, &db](const dpp::slashcommand_t& event) {
        string user_id = to_string(event.command.usr.id);

        if (event.command.get_command_name() == "register") {
            try {
                auto username_param = event.get_parameter("username");
                string username = std::get<string>(username_param);

                if (db.userExists(user_id)) {
                    string existing_username = db.getUsername(user_id);
                    event.reply("You are already registered as: " + existing_username);
                } else {
                    db.registerUser(user_id, username);
                    event.reply("Registered successfully as: " + username);
                }
            } catch (const exception& e) {
                cerr << "Registration error: " << e.what() << endl;
                event.reply("An error occurred during registration. Please try again.");
            }
        }
        else if (event.command.get_command_name() == "stats") {
    try {
        if (!db.userExists(user_id)) {
            event.reply(dpp::message("You need to register first! Use `/register` to get started.").set_flags(dpp::m_ephemeral));
            return;
        }

        // Get user data
        json user_data = db.getUserData(user_id);
        string username = user_data["username"];
        int level = user_data["level"];
        int xp = user_data["xp"];
        int gems = user_data["gems"];
        int wls = user_data["wls"];
        int growtokens = user_data["growtokens"];
        string farmable = user_data["farmable"];

        // Calculate XP needed for next level
        int B1 = level;
        int B2 = level + 1;
        int xpneeded = (50.0/6.0) * (B2*B2 * (B2-1) * (2*B2-1) - B1*B1 * (B1-1) * (2*B1-1)) + 100 * (B2-B1);

        dpp::user user = event.command.usr;
        string avatar_url = user.get_avatar_url(512);

        dpp::embed embed = dpp::embed()
            .set_color(0x00FF00)
            .set_title(username + "'s Stats")
            .set_thumbnail(avatar_url)
            .add_field("Level", to_string(level), true)
            .add_field("XP", to_string(xp), true)
            .add_field("Gems", to_string(gems), true)
            .add_field("XP to next level", to_string(xpneeded), true)
            .add_field("World locks", to_string(wls), true)
            .add_field("Growtokens", to_string(growtokens), true)
            .add_field("Farmable", farmable, true)
            .set_footer(dpp::embed_footer().set_text("User ID: " + user_id))
            .set_timestamp(time(0));

        event.reply(dpp::message().add_embed(embed));

    } catch (const exception& e) {
        cerr << "Stats error: " << e.what() << endl;
        event.reply("An error occurred while fetching your stats.");
    }
} else if (event.command.get_command_name() == "credits") {
    event.reply("Bot made by lietuvis_official");
}

//add more commands here
    });





    //Register commands here
    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            dpp::slashcommand register_cmd("register", "Register yourself", bot.me.id);
            register_cmd.add_option(
                dpp::command_option(dpp::co_string, "username", "Your desired username", true)
            );

            dpp::slashcommand stats_cmd("stats", "View your stats", bot.me.id);

            dpp::slashcommand addstat_cmd("addstat", "Add coins to your account", bot.me.id);
            addstat_cmd.add_option(
                dpp::command_option(dpp::co_integer, "addnum", "Number of coins to add", true)
            );

            dpp::slashcommand credits_cmd("credits", "Print credits", bot.me.id);

            bot.global_command_create(register_cmd);
            bot.global_command_create(stats_cmd);
            bot.global_command_create(addstat_cmd);
            bot.global_command_create(credits_cmd);

            cout << "Commands registered! Bot is ready." << endl;
        }
    });

    bot.start(dpp::st_wait);

    return 0;
}
