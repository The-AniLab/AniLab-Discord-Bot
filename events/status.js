const { Events, ActivityType } = require('discord.js');

module.exports = {
    name: Events.ClientReady,

    execute(client)
    {
        client.user.setPresence({ 
            activities: [{ 
                name: 'AniLab', 
                type: ActivityType.Watching 
            }], status: 'dnd' 
        });
    }
};