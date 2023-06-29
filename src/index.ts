import * as dotenv from 'dotenv'
import { Client, Events, GatewayIntentBits } from 'discord.js'

export const client = new Client({ intents: [
    GatewayIntentBits.Guilds,
    GatewayIntentBits.MessageContent,
] });

dotenv.config();

client.once(Events.ClientReady, () => {
    console.log(`${client.user!.tag} is online`);
});

client.login(process.env.TOKEN);