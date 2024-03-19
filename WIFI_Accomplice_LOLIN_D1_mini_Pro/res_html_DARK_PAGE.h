const char raw_html_DARK_PAGE[] PROGMEM =
R"=====(
<!DOCTYPE HTML>
<html>
<head>
    <meta http-equiv="Content-Type" Content="text/html; charset=utf-8">

    <link rel="icon" href="https://i.imgur.com/8zxaaMr.png">
	<title>USB HIJACKER</title>

    <!-- jQuery -->
    <script src="https://code.jquery.com/jquery-latest.min.js"></script>

    <!-- Bootstrap -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-T3c6CoIi6uLrA9TneNEoa7RxnatzjcDSCmG1MXxSR1GAsXEV/Dwwykc2MPK8M2HN" crossorigin="anonymous">
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/js/bootstrap.bundle.min.js" integrity="sha384-C6RzsynM9kWDrMNeT87bh95OGNyZPhcTNXj1NW7RuBCsyN/o0jlpcV8Qyq46cDfL" crossorigin="anonymous"></script>

    <!-- Split.js -->
    <script src="https://unpkg.com/split.js/dist/split.min.js"></script>

    <!-- sweetalert2 -->
    <script src="https://cdn.jsdelivr.net/npm/sweetalert2@11"></script>
    <link href="https://cdn.jsdelivr.net/npm/@sweetalert2/theme-dark@5.0.16/dark.min.css" rel="stylesheet">

    <!-- TypewriterJS -->
    <script src="https://unpkg.com/typewriter-effect@latest/dist/core.js"></script>

    <!-- Tippy.js -->
    <script src="https://unpkg.com/@popperjs/core@2"></script>
    <script src="https://unpkg.com/tippy.js@6"></script>

    <!-- DataTables -->
    <script src="https://cdn.datatables.net/1.10.19/js/jquery.dataTables.min.js"></script>
    <script src="https://cdn.datatables.net/1.10.19/js/dataTables.bootstrap.min.js"></script>

    <!-- Styles -->
    <link href="/DarkJunction/getStyles" rel="stylesheet">
</head>
<body>
    <div class="container-fluid" style="height: 100vh; padding: 2rem; background-color: #212121;">

        <div id="divPanelL" class="panelLR heightMax">

            <div id="divTerminal" class="heightMax"></div>

            <table id="tbDirectory" class="widthMax table table-dark table-striped table-hover" style="margin: 0 0; border-collapse: collapse;">
                <thead style="height: 3rem;">
                    <tr><th>FileName</th><th>FileSize</th></tr>
                </thead>
                <tbody class="table-group-divider" style="border-top-color: #CCCCCC">
                </tbody>
            </table>

        </div>

        <div id="divPanelR" class="panelLR heightMax">

            <textarea id="taFileContent" data-current-filename="" class="widthMax heightMax" spellcheck="false"></textarea>

            <button id="btnCreateFile" class="btnTextareaRB btnTextareaRB-Dark">+</button>
            <button id="btnSaveFile" class="btnTextareaRB btnTextareaRB-Gray"><svg xmlns="http://www.w3.org/2000/svg" width="22" height="22" fill="currentColor" class="bi bi-floppy" viewBox="0 0 16 16"><path d="M11 2H9v3h2z"/><path d="M1.5 0h11.586a1.5 1.5 0 0 1 1.06.44l1.415 1.414A1.5 1.5 0 0 1 16 2.914V14.5a1.5 1.5 0 0 1-1.5 1.5h-13A1.5 1.5 0 0 1 0 14.5v-13A1.5 1.5 0 0 1 1.5 0M1 1.5v13a.5.5 0 0 0 .5.5H2v-4.5A1.5 1.5 0 0 1 3.5 9h9a1.5 1.5 0 0 1 1.5 1.5V15h.5a.5.5 0 0 0 .5-.5V2.914a.5.5 0 0 0-.146-.353l-1.415-1.415A.5.5 0 0 0 13.086 1H13v4.5A1.5 1.5 0 0 1 11.5 7h-7A1.5 1.5 0 0 1 3 5.5V1H1.5a.5.5 0 0 0-.5.5m3 4a.5.5 0 0 0 .5.5h7a.5.5 0 0 0 .5-.5V1H4zM3 15h10v-4.5a.5.5 0 0 0-.5-.5h-9a.5.5 0 0 0-.5.5z"/></svg></button>

            <div id="divFileSelector"></div>

        </div>

    </div>

    <!-- Scripts -->
    <script src="/DarkJunction/getScripts"></script>
</body>
</html>
)=====";

#include "res_css_DARK_PAGE.h"
#include "res_js_DARK_PAGE.h"