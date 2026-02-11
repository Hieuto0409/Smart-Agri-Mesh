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
    margin: 50px 20px 20px 20px;
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
.ValueContain{
    display: grid;
    grid-template-columns: repeat(3, 1fr); /* Chia 3 cột đều tăm tắp */
    gap: 20px; /* Khoảng cách giữa các khối */
    padding: 0 50px 0 50px;
}
</style>
</head>
<body>
<div class="header">
<h1 id="header">HỆ THỐNG TƯỚI TIÊU</h1>
<p class="author">Dự án IoT - Tô Quang Hiếu</p>
    
</div>
<div class="ValueContain">
    <div class="humiValue">
        <span class="icon">☀️</span>
        <h1 class="humiValue_h1">Ánh sáng</h1>
        <div class="humiValueContain">
            <p id="LightValue_p">50</p><span>%</span>
            
        </div>
    </div>
    <div class="humiValue">
        <span class="icon">🌱</span>
        <h1 class="humiValue_h1">Độ ẩm</h1>
        <div class="humiValueContain">
            <p id="humiValue_p">50</p><span>%</span>
            
        </div>
    </div>
    <div class="humiValue">
        <span class="icon">🌡</span>
        <h1 class="humiValue_h1">Nhiệt độ</h1>
        <div class="humiValueContain">
            <p id="TempValue_p">23</p><span>℃</span>
            
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
    let LightValue= document.getElementById('LightValue_p');
    let TemptValue= document.getElementById('TempValue_p');
    let pump = document.getElementById('button_Bt');
    let check = 0;
    let on = 1;
    let off = 0;
    
    function WaterPump (){
        if (check == 0) {
            pump.style.backgroundColor="red";
            pump.innerText="Hủy bơm nước";
            check =1;
            fetch("/Pumpstatus?Status="+off);
        }else{
            pump.style.backgroundColor="#3bb870";
            pump.innerText="Bơm nước";
            check = 0;
            fetch("/Pumpstatus?Status="+on);
        }
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
        fetch("/GetLightValue")
        .then(respond => respond.text())
        .then(data => {
            LightValue.innerText = data;
        })
        .catch(error => {
            console.log("Lỗi rồi: " + error);
        })
        fetch("/GetTempValue")
        .then(respond => respond.text())
        .then(data => {
            TemptValue.innerText = data;
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