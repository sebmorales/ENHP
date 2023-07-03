let express = require('express');
let app = express();
let server = app.listen(process.env.PORT || 8877);
console.log('server running')
var fs = require('fs');
var ejs = require('ejs');
//Setup public lib
app.use(express.static(__dirname + '/public'));
//Setup the views folder
app.set("views", __dirname + '/views');

//Setup ejs, so I can write HTML (:
app.engine('.html', ejs.__express);
app.set('view-engine', 'html');

let boards=[];

var socket = require('socket.io');
var io = socket(server, {
    //this allows external websites to connect
  cors: {
    origin: '*'
  },
  //this allows older socket versions to connect
  allowEIO3: true
});

io.sockets.on('connection', newConnection);



//Router
app.get("/", function(req, res){
    res.render("windows.html")
});
app.get("/playback", function(req, res){
  res.render("playback.html")
});

function newConnection(socket){
  io.emit("photo",boards);

  console.log("new Connection")
  socket.on("message", function(data){
    //this will send to everyone connected to the site
    io.emit("message", data)
    // socket_client.emit("message",data);
    // socket_nuum.emit("message",data);
    // console.log(data)

    // if(data.da>0.1){
    //   // data.bat=data.bat*2.0/4095.0;
    // console.log(data)
    // }
  })

   socket.on("midi", function(data){
     // console.log(data);
     io.emit("midi",data)
  })

   socket.on("osc", function(data){
     // console.log(data);
     io.emit("osc",data)
  })

  socket.on("photo",function(data){
    // console.log(data.name)
    console.log(data);
    let board_exists=false;
    data.time=Date.now();
    for (let i=0;i<boards.length;i++){
      if(boards[i].name==data.name){
        boards[i].photo=data.photo;
        boards[i].time=data.time;
        board_exists=true;
      }
 
    }
    if(!board_exists){
      let new_board={
        "name":data.name,
        "photo":data.photo,
        "time":data.time
      }
      boards.push(new_board);
    }
    fs.appendFile('public/js/photo_history.txt', "\n"+JSON.stringify(data), function (err) {
    if (err) throw err;
      // console.log("data saved");
     });
     io.emit("photo",boards);
    //  console.log(data);
  });
}
