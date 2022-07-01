//tattoo.live

//Express
var express = require('express');
var app = express();
var https = require('https');

var http = require('http').createServer(app);
var port = process.env.PORT || 8080;//8099
// var port = process.env.PORT || 4000;//8099
var server = app.listen(port);
var fs = require('fs');
var bodyParser= require("body-parser");
//ejs
var ejs = require('ejs');

let window_names=["kale","ginger","onion","cilantro","cabbage","carrot","broccoli","mushroom","potato","tomato","beet","pepper","cauliflower","celery","avocado","spinach","radish","basil","mint","parsley"]
var windows=[]

//Sockets
var socket = require('socket.io').listen(server);
console.log("running on port "+port)

//Setup public lib
app.use(express.static(__dirname + '/public'));
//Setup the views folder
app.set("views", __dirname + '/views');

//Setup ejs, so I can write HTML (:
app.engine('.html', ejs.__express);
app.set('view-engine', 'html');

app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());


//Router
app.get("/", function(req, res){
    console.log("here");
    res.render("windows.html")
});

app.get("/morakana", function(req, res){
    res.render("morakana.html")
});



socket.on('connection', function(client){
    console.log("new connection");
    socket.emit('allWindos',windows);
    client.on('getSetup', function(data){
      socket.emit('allWindos',windows);

    });




    client.on('photoresistor', function(data){
        console.log(data);
        for(let i=0;i<20;i++){
          if("photo_"+windows[i].name==data.board){
            windows[i].value=data.photo;
            let d={};
            d.name=windows[i].name;
            d.value=windows[i].value;
            d.time=Date.now();
            fs.appendFile('light_history.txt', JSON.stringify(d), function (err) {
            if (err) throw err;
              //console.log('Saved!');
            });
          }
        }
        socket.emit('photoresistor', data);
    });


    client.on('disconnect', function(){
        console.log("A user dissconected.");
    });
});


class Window{
  constructor(name, value){
    this.name=name;
    this.value=value;
  }
 }
 initWindows();
 function initWindows(){
     for (let i=0;i<window_names.length;i++){
         windows.push(new Window(window_names[i],-1));
     }
     console.log(windows)
 }
