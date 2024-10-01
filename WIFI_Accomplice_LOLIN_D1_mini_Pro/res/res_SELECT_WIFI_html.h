const char raw_html_SELECT_WIFI[] PROGMEM =
R"=====(
<!DOCTYPE HTML>
<html>
<head>
    <meta http-equiv="Content-Type" Content="text/html; charset=utf-8">
    <meta name='viewport' content='width=device-width, initial-scale=1, minimum-scale=1, maximum-scale=1, user-scalable=no'>

    <title>WiFi List</title>

    <style>
        body {font-size:1.2rem;}
        a:hover {cursor:pointer;}

        #aCollapsible {display:none;text-decoration:underline;}

        #spinner {z-index:9;position:fixed;top:0;left:0;right:0;bottom:0;background-color:rgba(0, 0, 0, 0.7);padding:0 auto;display:none;}
        #spinner .spinner-around {z-index:99;position:absolute;top:50%;left:50%;margin:-25px 0 0 -25px;width:50px;height:50px;border-radius:50%;border:7px solid rgba(0, 0, 0, 0.1);animation:none;}
        #spinner .spinner-inner {z-index:999;position:absolute;top:50%;left:50%;margin:-25px 0 0 -25px;width:50px;height:50px;border-radius:50%;border:7px solid transparent;border-top-color:#3498db;}
    </style>
</head>
<body>
    <h1>WiFi List</h1>
    인증가능한 WiFi를 선택하세요. <br/>

    <div id='wifiList'>
        <ul>
            #{strWiFiList}
            <a id='aCollapsible'>...</a>
        </ul>
    </div>

    <div id='divForm'>
        <form>
            SELECTED SSID : <br>
            <input type='text' name='apName' readonly> <br>
            PASSWORD(CAN BE NONE) : <br>
            <input type='password' name='apPw'> <br>
        </form>
        <button id="btnConnect">CONNECT</button>
    </div>

    <div id='spinner'>
        <span class='spinner-around'></span>
        <span class='spinner-inner'></span>
    </div>
</body>
<script>
    const divWifiList = document.getElementById('wifiList');

    const divForm = document.getElementById('divForm');
    divForm.style.visibility = 'hidden';

    const arrLiApName = divWifiList.querySelectorAll('li');
    for (liApName of arrLiApName) {
        liApName.querySelector('a').addEventListener("click", function()
        {
            for (liApName of arrLiApName)
                liApName.style.color = (liApName === this.parentNode) ? 'navy' : 'lightgray';

            divForm.style.visibility = 'visible';
            divForm.querySelector('[name="apName"]').value = this.textContent.trim();
            divForm.querySelector('[name="apPw"]').value = ''
        });
    }

    const aCollapsible = divWifiList.querySelector('#aCollapsible');
    if(arrLiApName.length > 10)
    {
        for(let i=0; i<arrLiApName.length; i++)
            arrLiApName[i].style.display = (i>7) ? 'none' : '';

        aCollapsible.style.display='block';
        aCollapsible.addEventListener("click", () =>
        {
            for (liApName of arrLiApName)
                liApName.style.display = '';
                
            aCollapsible.style.display = 'none';
        });
    }
</script>
<script>
    let responseLatest = '';
    function postAjax(funcSuccess, url, data = null)
    {
        let xhr = new XMLHttpRequest();
        xhr.onreadystatechange = function() {
            if (xhr.readyState === XMLHttpRequest.DONE)
            {
                if (xhr.status === 200)
                {
                    responseLatest = xhr.responseText;
                    funcSuccess();
                }
                else { console.error('Error:', xhr.status); }
            }
        };
        xhr.open('POST', url, true);

        if(data == null)
            xhr.send();
        else {
            xhr.setRequestHeader('Content-Type', 'application/json');
            xhr.send( JSON.stringify(data) );
        }

        return xhr;
    }

    // for, CONNECT TO NET!
    document.querySelector('#btnConnect').addEventListener("click", function()
    {
        showSpinner();

        postAjax(   function(){ window.location.href = ("/isConnected?apName=" + divForm.querySelector('[name="apName"]').value); }
                    , '/connectToAP'
                    , { apName: divForm.querySelector('[name="apName"]').value, apPw: divForm.querySelector('[name="apPw"]').value }
                );
    });

    // for, the case of refresh the page
    postAjax(   function(){}
                , '/rescanWifiList'
            );

    // for, automatically select the previously chosen apName
    const params = new URLSearchParams(window.location.search);
    const param_apName = params.get("apName");
    if (param_apName !== null) {
        for(a of divWifiList.querySelectorAll('a'))
            if(a.textContent.includes(param_apName))
                a.dispatchEvent(new MouseEvent('click', { bubbles: true, cancelable: true }));
    }
</script>
<script>
    //spinner made by ChatGPT
    const spinnerInner = spinner.querySelector('.spinner-inner');
    let degree = 0;
    
    const animateSpinner = () => {
        degree = (degree+5) % 360;

        spinnerInner.style.transform = `rotate(${degree}deg)`;
        spinnerInner.style.borderLeftColor = '#3498db';

        setTimeout(() => { spinnerInner.style.borderLeftColor = 'transparent'; animateSpinner(); }, 20);
    };
    animateSpinner();

    function showSpinner()
    {   document.querySelector('#spinner').style.display = 'block';   }
    function hideSpinner()
    {   document.querySelector('#spinner').style.display = 'none';   }
</script>
</html>
)=====";

String strWiFiList = "";
