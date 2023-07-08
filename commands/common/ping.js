const { SlashCommandBuilder } = require('discord.js');

module.exports = {
	data: new SlashCommandBuilder()
		.setName('ping')
		.setDescription('Check bot latency'),
	async execute(interaction) {
		return interaction.reply('Pong!');
	},
};