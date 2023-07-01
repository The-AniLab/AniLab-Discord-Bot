import { ActivityType, Client } from 'discord.js';

export const name = 'ready';

export function execute(client: Client) {
    client.user?.setPresence({ activities: [{ name: 'AniLab', type: ActivityType.Watching }], status: 'dnd' });
}