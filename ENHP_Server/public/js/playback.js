
let boards=[];
let OSCs=[];
let min_w;
let max_col=7;

let history_raw;
let history_parsed=[];
let runTime;
let timeOffset;
let speed=1000000;//1000=1Sec 


let swiper={speed:10, x:0}
let sonify=false;

function preload(){
  createCanvas(windowWidth, windowHeight);
  history_raw=loadStrings("/js/photo_history.txt");
}


function setup() {
  createCanvas(windowWidth, windowHeight);
  for (let i=0;i<history_raw.length;i++){
    history_parsed[i]=JSON.parse(history_raw[i]);
  }
  console.log("done parsing")
  background(20);
  runTime=history_parsed[0].time;
  console.log(runTime)
}

function mouseClicked(){
  console.log(runTime+millis()*speed);
  console.log(history_parsed[0].time+timeOffset)
  sonify=!sonify;
}


function draw() {
  if(runTime+(millis()*speed)>=history_parsed[0].time){
      photoEvent(history_parsed[0]);
      // console.log(history_parsed[0])
    history_parsed.shift()
    if(history_parsed.length<1){
      console.log("DONE!");
    }
  }
  if(sonify){
      stroke(255);
      strokeWeight(3);
      line(swiper.x,0,swiper.x,height);
      swiper.x+=swiper.speed;
      if(swiper.x>width){
        swiper.x=0;
      }
    }


}



function photoEvent(data) {
    let board_exists=false;
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
        "time":data.time,
        "playing":false
      }
      boards.push(new_board);
      OSCs.push(new p5.Oscillator('sine'));

    }  
  draw_boards();
}


function windowResized() {
  resizeCanvas(windowWidth, windowHeight);
  min_w=width/max_col;


};


function draw_boards() {
  background(0);
  let t_s=map(width,0,1000,4,15);
  textSize(t_s);
  let b_n=boards.length;
  let col_w;
  if(b_n>max_col){
    col_w=width/max_col;
  }else{
    col_w=width/b_n;
  }
 
  let row_n=ceil(b_n/max_col);
  let row_h=height/row_n;

  for(let i=0; i<boards.length;i++){
    let c=i%max_col;
    let r=floor(i/max_col);
    // if (r<0){r=0;}
    let min = color(0, 26, 13);
    let max = color(0, 220, 60);
    noStroke();
    fill(lerpColor(min,max,boards[i].photo))
    rect(c*col_w,r*row_h,col_w,row_h);
    if(sonify){
      if( c*col_w<swiper.x && swiper.x<c*col_w+col_w ){
        freq=map(boards[i].photo,0,1,120,500);
        OSCs[i].freq(freq,0.1);
        fill(0,0,map(boards[i].photo,0,1,0,255));
        rect(c*col_w,r*row_h,col_w,row_h);
        if(!boards[i].playing ){
          OSCs[i].pan(map(height/width,.2,3,-1,1));
          OSCs[i].amp(.2,0.2);
          OSCs[i].start();
        }
        boards[i].playing=true;

        // }
      }else{
        if(boards[i].playing){
          OSCs[i].amp(0,0.2)
          // OSCs[i].stop(.2)
          boards[i].playing=false;
        }
      }
    }
    fill(255);
    text(boards[i].name, c*col_w+t_s,row_h*r+t_s+5);
  }

}

