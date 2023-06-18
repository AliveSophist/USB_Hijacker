const char rawhtml_SELECT_WIFI[] PROGMEM = 
R"(
<!DOCTYPE HTML>
<html>
<head>
    <meta http-equiv="Content-Type" Content="text/html; charset=utf-8">
    <meta name='viewport' content='width=device-width, initial-scale=1, minimum-scale=1, maximum-scale=1, user-scalable=no'>

    <title>WiFi 목록</title>

    <style>
    body
    {font-size:1.2rem;}

    .a:hover
    {cursor:pointer;}

    #spinner
    {z-index:9;position:fixed;top:0;left:0;right:0;bottom:0;background-color:rgba(0, 0, 0, 0.7);padding:0 auto;display:none;}
    #spinner .spinner-around
    {z-index:99;position:absolute;top:50%;left:50%;margin:-25px 0 0 -25px;width:50px;height:50px;border-radius:50%;border:7px solid rgba(0, 0, 0, 0.1);animation:none;}
    #spinner .spinner-inner
    {z-index:999;position:absolute;top:50%;left:50%;margin:-25px 0 0 -25px;width:50px;height:50px;border-radius:50%;border:7px solid transparent;border-top-color:#3498db;}
    </style>
</head>
<body>
    <h1>WiFi List</h1>
    인증가능한 WiFi를 선택하세요. <br>

    <div id='wifiList'>#{strWiFiList}</div>

    <div id='divForm'>
        <form method='post' action='/connectToAP' onsubmit='return whenSubmit();'>
            SELECTED SSID : <br>
            <input type='text' name='apName' readonly> <br>
            PASSWORD(CAN BE NONE) : <br>
            <input type='password' name='apPw'> <br>
            <input type='submit' value='CONNECT'>
        </form>
    </div>

    <div id='spinner'>
        <span class='spinner-around'></span>
        <span class='spinner-inner'></span>
    </div>
</body>
<script>
    const divForm = document.getElementById('divForm');
    divForm.style.visibility = 'hidden';

    const arrLiApName = document.querySelectorAll('.liApName');
    const aSummary = document.querySelector('#aSummary');
    if(arrLiApName.length > 10)
    {
        aSummary.style.display='block';
        aSummary.addEventListener("click", () => { for (liApName of arrLiApName) liApName.style.display=''; aSummary.style.display='none'; });
    }
    let i = 1;
    for (liApName of arrLiApName) {
        const aTag = liApName.querySelector('a');
        aTag.addEventListener("click", () => { select(aTag.name); });

        if(i++>10) liApName.style.display='none';
    }
    function select(apName){
        for (liApName of arrLiApName) {
            if(liApName.querySelector('a').name === apName)
                liApName.style.color = 'navy';
            else
                liApName.style.color = 'gray';
        }

        divForm.style.visibility = 'visible';
        divForm.querySelector('[name="apName"]').value = apName;
        divForm.querySelector('[name="apPw"]').value = ''
    }
</script>
<script>
    //spinner by ChatGPT
    const spinnerInner = spinner.querySelector('.spinner-inner');
    let degree = 0;
    
    const animateSpinner = () => {
        degree = (degree+5) % 360;

        spinnerInner.style.transform = `rotate(${degree}deg)`;
        spinnerInner.style.borderLeftColor = '#3498db';

        setTimeout(() => { spinnerInner.style.borderLeftColor = 'transparent'; animateSpinner(); }, 20);
    };
    animateSpinner();

    function whenSubmit(){
        document.querySelector('#spinner').style.display = 'block';
        return true;
    }
</script>
</html>
)";