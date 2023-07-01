import fs from 'node:fs';
import path from 'node:path';

import { Client } from 'discord.js';

export function eventHandler(client: Client) 
{
    const eventPath = path.join(__dirname, '../events');
    const eventFiles = fs.readdirSync(eventPath).filter(file => file.endsWith('.js'));

    for (const file of eventFiles) 
    {
        const filePath = path.join(eventPath, file);
        const event = require(filePath);

    if (event.once) 
        client.once(event.name, (...args: any) => event.execute(...args, client));
    else
        client.on(event.name, (...args: any) => event.execute(...args, client));
    }
}