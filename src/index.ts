import * as dotenv from 'dotenv'
import { Client, GatewayIntentBits } from 'discord.js'
import { eventHandler } from '../handlers/event_handler';

dotenv.config();

export const client = new Client({ intents: [
    GatewayIntentBits.Guilds,
    GatewayIntentBits.MessageContent,
] });

eventHandler(client);

client.login(process.env.TOKEN);