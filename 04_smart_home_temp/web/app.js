var express = require("express");
var pg = require("pg");
var bodyParser = require("body-parser");

var CON_STRING = process.env.DB_CON_STRING;
if (CON_STRING == undefined) {
    console.log("Error: Environment variable DB_CON_STRING not set!");
    process.exit(1);
}

pg.defaults.ssl = true;
var dbClient = new pg.Client(CON_STRING);
dbClient.connect();

var urlencodedParser = bodyParser.urlencoded({
    extended: false
});

const PORT = 3000;

var app = express();

app.use(express.static('public'));

app.get("/sensor/temp", function (req, res) {
    dbClient.query("SELECT * FROM temperatures", function (dbError, dbResponse) {
        res.send(JSON.stringify(dbResponse.rows));
    });
});

app.post("/sensor/temp", urlencodedParser, function(req, res) {
  console.log(`temperature ${req.body.temperature} received`);
  var temperature = req.body.temperature;
  var room = req.body.room;
  dbClient.query("INSERT INTO temperatures (timestamp, temperature, room) VALUES (now(), $1, $2)", [temperature, room], function(dbError, dbResponse) {
    console.log("temperature measurement added to database");
  });
});

app.listen(PORT, function () {
    console.log(`Server listening on Port ${PORT}`);
});
