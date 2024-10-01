const char raw_html_INDEX[] PROGMEM =
R"=====(
<!DOCTYPE HTML>
<html>
<head>
	<title>USB HIJACKER</title>
</head>
<body>
	<h1 id="content">HACKED</h1>
</body>
<script>
    const content = "${content}";
    if(!content.includes("{content}")) // if replaced by controller
        document.querySelector('#content').innerHTML = content;
</script>
</html>
)=====";
