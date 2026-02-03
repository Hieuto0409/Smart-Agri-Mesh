#ifndef WEBPAGE_H // kiem tra ten duoc dinh nghia chua
#define WEBPAGE_H // dinh nghia file

#include <Arduino.h>
// PROGMEM de chuoi html trong bo nho Flash
const char html_page[] PROGMEM = R"====(<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Smart-Agri-Mesh</title>
<style>
    * {
    margin: 0;
    padding: 0;
    box-sizing: border-box; 
    font-family: 'Arial', sans-serif;
    text-align: center;
}
    #header {
        color: #FFF;
}
.header{
        background-color:  #2a9a59;
        padding-top:20px ;
        padding-bottom:20px;
}
.author{
    text-align: center;
    color: #FFF;
}
.humiValue{
    margin: 50px 200px 20px 200px;
    border-radius: 10px;
    box-shadow: 0 2px 10px rgba(0,0,0,0.5);
    padding: 20px;
}
.humiValue_h1{
    padding: 20px;
    
}
.icon{
    font-size: 50px;
}
.button{
    /*border: 1px solid rgba(0,0,0,1);*/
    padding: 20px;
    margin: 50px 200px 20px 200px;
}
#button_Bt{
    background-color: #3bb870;
    color: #FFF;
    padding: 20px 50px 20px 50px;
    border-radius: 10px;
    border: none;
    font-size: 20px;
}
#button_Bt:hover{
    cursor: pointer;
    background-color: #2c7e4f;
}
.humiValueContain{
    font-size: 30px;
    display: flex;              /* Kích hoạt chế độ dàn hàng ngang */
    align-items: baseline;      /* Cực hay: Giúp chân chữ "50" và "%" thẳng hàng nhau */
    justify-content: center;
}
</style>
</head>
<body>
<div class="header">
<h1 id="header">HỆ THỐNG TƯỚI TIÊU</h1>
<p class="author">Dự án IoT - Tô Quang Hiếu</p>
    
</div>
<div>
    <div class="humiValue">
        <span class="icon">🌱</span>
        <h1 class="humiValue_h1">Giá trị độ ẩm</h1>
        <div class="humiValueContain">
            <p id="humiValue_p">50</p><span>%</span>
            
        </div>
    </div>
</div>
<div>
    <div class="button">
        <button id="button_Bt" onclick="WaterPump()" type="submit">Bơm nước</button>
    </div>
</div>
<script>
    let humiValue= document.getElementById('humiValue_p');
    let pump = document.getElementById('button_Bt');
    
    function WaterPump (){
        pump.style.backgroundColor="red";
        var On = 1;
        fetch("/Pumpstatus?Status="+On);
    }
    function GetValue (){
        fetch("/GetHumiValue")
        .then(respond => respond.text())
        .then(data => {
            humiValue.innerText = data;
        })
        .catch(error => {
            console.log("Lỗi rồi: " + error);
        })
    }
    setInterval(GetValue, 500);
</script>
</body>
</html>)====";
#endif