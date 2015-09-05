var commander = require("commander"),
	spawn = require('child_process').spawn,
	WebSocket = require("ws");

commander
  .version('0.0.1')
  .option('-s, --server [ip:port]', 'WebSocket server')
  .option('-c, --command [command]', 'Command to execute')
  .parse(process.argv);

var child = spawn(commander.command, []).on('error', function( err ){ throw err });
var ws = new WebSocket("ws://" + commander.server);
ws.on('open', function () {
	console.log("Connected");
	child.stdout.on('data', function(data) {
	  ws.send(data.toString());
	});
});

ws.on("error", function(err){
	console.log("error " + err);
});