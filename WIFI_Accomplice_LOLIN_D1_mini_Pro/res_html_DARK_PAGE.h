const char raw_html_DARK_PAGE[] PROGMEM =
R"=====(
<!DOCTYPE HTML>
<html>
<head>

    <meta http-equiv="Content-Type" Content="text/html; charset=utf-8">

    <link rel="icon" href="https://i.imgur.com/8zxaaMr.png">
	<title>USB HIJACKER</title>

    <script>
        // hash exists? Invalid access. redirect to original url.
        var hash = window.location.hash;
        if (hash !== '') window.location.href = window.location.href.replace(hash, '');
    </script>

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

    <div id="pagepiling" class="container-fluid g-0" style="display: none;">


        <div id="pageEditor" class="section">
        <div style="width:100vw; height: 100vh; padding: 2rem; background-color: #212121;">

            <div id="divPanelL" class="panelLR heightMax">

                <div id="divTerminal" class="heightMax"></div>

                <table id="tbDirectory" class="widthMax table table-dark table-striped table-hover" style="margin: 0 0; border-collapse: collapse;">
                    <thead style="height: 3rem;">
                        <tr><th>FileName</th><th>FileSize</th></tr>
                    </thead>
                    <tbody class="table-group-divider" style="border-top-color: #CCCCCC"></tbody>
                </table>

            </div>

            <div id="divPanelR" class="panelLR heightMax">

                <div id="divFileList"></div>

                <textarea id="taFileContent" data-current-filename="" class="widthMax heightMax" spellcheck="false"></textarea>

                <!-- Go To Listener -->
                <button id="btnGoToListener" onclick="location.href='#pageListener'" class="btnTogglePage btnTogglePage-R"></button>
            </div>

            <button id="btnCreateFile" class="btnBottomRight"></button>
            <button id="btnSaveFile" class="btnBottomRight"></button>

        </div>
        </div>


        <div id="pageListener" class="section">
        <div style="width:100vw; height: 100vh; padding: 2rem; background-image: url(https://i.imgur.com/yBtUH6Y.jpg); background-size: contain;">

            <div class="panelListener heightMax justify-content-center">
                <!-- Go To Editor -->
                <button id="btnBackToEditor" onclick="location.href='#pageEditor'" class="btnTogglePage btnTogglePage-L"></button>

                <div class="row justify-content-center" style="min-height: 270px; max-height: 270px; font-weight: bold;">
                    <div class="keyboard col-12 d-flex justify-content-center" style="min-width: 1100px; /*height: 270px;*/"> <section> <div class="row"> <div id="key_esc" class="key">ESC</div> <div class="key_filler"></div> <div id="key_f1" class="key">F1</div> <div id="key_f2" class="key">F2</div> <div id="key_f3" class="key">F3</div> <div id="key_f4" class="key">F4</div> <div class="key_filler"></div> <div id="key_f5" class="key">F5</div> <div id="key_f6" class="key">F6</div> <div id="key_f7" class="key">F7</div> <div id="key_f8" class="key">F8</div> <div class="key_filler"></div> <div id="key_f9" class="key">F9</div> <div id="key_f10" class="key">F10</div> <div id="key_f11" class="key">F11</div> <div id="key_f12" class="key">F12</div> </div> <div class="row"> <div id="key_accent" class="key"><em>~</em><br>`</div> <div id="key_one" class="key"><em>!</em><br>1</div> <div id="key_two" class="key"><em>@</em><br>2</div> <div id="key_three" class="key"><em>#</em><br>3</div> <div id="key_four" class="key"><em>$</em><br>4</div> <div id="key_five" class="key"><em>%</em><br>5</div> <div id="key_six" class="key"><em>^</em><br>6</div> <div id="key_seven" class="key"><em>&amp;</em><br>7</div> <div id="key_eight" class="key"><em>*</em><br>8</div> <div id="key_nine" class="key"><em>(</em><br>9</div> <div id="key_zero" class="key"><em>)</em><br>0</div> <div id="key_hyphen" class="key"><em>_</em><br>-</div> <div id="key_equals" class="key"><em>+</em><br>=</div> <div id="key_backspace" class="key wide_2"><span class="right">backspace</span></div> </div> <div class="row"> <div id="key_tab" class="key wide_2"><span class="left">tab</span></div> <div id="key_q" class="key single">Q</div> <div id="key_w" class="key single">W</div> <div id="key_e" class="key single">E</div> <div id="key_r" class="key single">R</div> <div id="key_t" class="key single">T</div> <div id="key_y" class="key single">Y</div> <div id="key_u" class="key single">U</div> <div id="key_i" class="key single">I</div> <div id="key_o" class="key single">O</div> <div id="key_p" class="key single">P</div> <div id="key_left_bracket" class="key"><em>{</em><br>[</div> <div id="key_right_bracket" class="key"><em>}</em><br>]</div> <div id="key_backslash" class="key"><em>|</em><br>\</div> </div> <div class="row"> <div id="key_caps_lock" class="key wide_3"><span class="left">caps lock</span></div> <div id="key_a" class="key single">A</div> <div id="key_s" class="key single">S</div> <div id="key_d" class="key single">D</div> <div id="key_f" class="key single">F</div> <div id="key_g" class="key single">G</div> <div id="key_h" class="key single">H</div> <div id="key_j" class="key single">J</div> <div id="key_k" class="key single">K</div> <div id="key_l" class="key single">L</div> <div id="key_semicolon" class="key"><em>:</em><br>;</div> <div id="key_apostrophe" class="key"><em>"</em><br>'</div> <div id="key_enter" class="key wide_3"><span class="right">enter</span></div> </div> <div class="row"> <div id="key_left_shift" class="key wide_4"><span class="left">shift</span></div> <div id="key_z" class="key single">Z</div> <div id="key_x" class="key single">X</div> <div id="key_c" class="key single">C</div> <div id="key_v" class="key single">V</div> <div id="key_b" class="key single">B</div> <div id="key_n" class="key single">N</div> <div id="key_m" class="key single">M</div> <div id="key_comma" class="key"><em>&lt;</em><br>,</div> <div id="key_period" class="key"><em>&gt;</em><br>.</div> <div id="key_forwardslash" class="key"><em>?</em><br>/</div> <div id="key_right_shift" class="key wide_4"><span class="right">shift</span></div> </div> <div class="row"> <div id="key_left_ctrl" class="key wide_1"><span class="left">ctrl</span></div> <div id="key_left_gui" class="key wide_1"><span class="left">gui</span></div> <div id="key_left_alt" class="key wide_1"><span class="left">alt</span></div> <div id="key_space" class="key wide_5"></div> <div id="key_right_alt" class="key wide_1"><span class="right">alt</span></div> <div id="key_right_app" class="key wide_1"><span class="right">app</span></div> <div id="key_right_ctrl" class="key wide_1"><span class="right">ctrl</span></div> </div> </section> <section> <div class="row"> <div id="key_print" class="key"><span>print</span></div> <div id="key_scroll_lock" class="key"><span>scroll lock</span></div> <div id="key_pause_break" class="key"><span>pause break</span></div> </div> <div class="row"> <div id="key_insert" class="key"><span>insert</span></div> <div id="key_home" class="key"><span>home</span></div> <div id="key_page_up" class="key"><span>page up</span></div> </div> <div class="row"> <div id="key_delete" class="key"><span>delete</span></div> <div id="key_end" class="key"><span>end</span></div> <div id="key_page_down" class="key"><span>page down</span></div> </div> <div class="row"> </div> <div class="row"> <div class="key_filler"></div> <div id="key_up" class="key"><div class="triangle up"></div></div> <div class="key_filler"></div> </div> <div class="row"> <div id="key_left" class="key"><div class="triangle left"></div></div> <div id="key_down" z="" class="key"><div class="triangle down"></div></div> <div id="key_right" class="key"><div class="triangle right"></div></div> </div> </section> <section> <div class="row"> </div> <div class="row"> <div id="key_num_lock" class="key"><span>num lock</span></div> <div id="key_divide" class="key single">/</div> <div id="key_multiply" class="key single">*</div> <div id="key_subtract" class="key single">-</div> </div> <div class="row"> <div id="key_num_7" class="key">7<span>home</span></div> <div id="key_num_8" class="key">8<br><div class="triangle up"></div></div> <div id="key_num_9" class="key">9<span>pgup</span></div> <div id="key_add" class="key tall">+</div> </div> <div class="row"> <div id="key_num_4" class="key">4<br><div class="triangle left"></div></div> <div id="key_num_5" class="key">5</div> <div id="key_num_6" class="key">6<br><div class="triangle right"></div></div> </div> <div class="row"> <div id="key_num_1" class="key">1<span>end</span></div> <div id="key_num_2" class="key">2<br><div class="triangle down"></div></div> <div id="key_num_3" class="key">3<span>pgdn</span></div> <div id="key_num_enter" class="key tall"><span>enter</span></div> </div> <div class="row"> <div id="key_num_0" class="key wide_6">0<span class="right">insert</span></div> <div id="key_num_decimal" class="key">.<br>del</div> </div> </section> </div>
                </div>

                <div class="row justify-content-center" style="height: calc(100% - 270px - 4rem);">
                    <div class="row d-flex justify-content-center" style="width: 700px; padding-bottom: 2rem;">
                        <div class="col-5">
                            <select id="selectEventList" class="selectList" size="3" multiple></select>
                        </div>
                        <div id="divEventModifier" class="col-2 row d-flex justify-content-center flex-column">
                            <input id="inpStabilizeDelay" class="form-control" type="text" placeholder="STABILIZE DELAY" style="color: blue;">
                            <button id="btnAddDelay" class="btnEventModifier"></button>
                            <button id="btnMakeFunction" class="btnEventModifier"></button>
                            <button id="btnCallFunction" class="btnEventModifier"></button>
                            <button id="btnDeleteEvent" class="btnEventModifier"></button>
                            <button id="btnDeleteEventAll" class="btnEventModifier" style="position: relative;">
                                <span style="position: absolute; top: 25px; left: 20px; color: red; font-weight: bold; font-size: 15px;">All</span>
                            </button>
                        </div>
                        <div class="col-5">
                            <select id="selectFunctionList" class="selectList" size="3"></select>
                        </div>
                    </div>
                </div>

            </div>

        </div>
        </div>


    </div>


    <!-- Inject SVG -->
    <script>
        let svgCode;
        // svgCode = ``;
        // $('').prepend(svgCode);

        svgCode = `<svg width="24" height="24" xmlns="http://www.w3.org/2000/svg" fill="currentColor" class="bi bi-file-earmark-plus" viewBox="0 0 16 16"><path d="M8 6.5a.5.5 0 0 1 .5.5v1.5H10a.5.5 0 0 1 0 1H8.5V11a.5.5 0 0 1-1 0V9.5H6a.5.5 0 0 1 0-1h1.5V7a.5.5 0 0 1 .5-.5"/><path d="M14 4.5V14a2 2 0 0 1-2 2H4a2 2 0 0 1-2-2V2a2 2 0 0 1 2-2h5.5zm-3 0A1.5 1.5 0 0 1 9.5 3V1H4a1 1 0 0 0-1 1v12a1 1 0 0 0 1 1h8a1 1 0 0 0 1-1V4.5z"/></svg>`;
        $('#btnCreateFile').prepend(svgCode);
        svgCode = `<svg width="24" height="24" xmlns="http://www.w3.org/2000/svg" fill="currentColor" class="bi bi-floppy" viewBox="0 0 16 16"><path d="M11 2H9v3h2z"/><path d="M1.5 0h11.586a1.5 1.5 0 0 1 1.06.44l1.415 1.414A1.5 1.5 0 0 1 16 2.914V14.5a1.5 1.5 0 0 1-1.5 1.5h-13A1.5 1.5 0 0 1 0 14.5v-13A1.5 1.5 0 0 1 1.5 0M1 1.5v13a.5.5 0 0 0 .5.5H2v-4.5A1.5 1.5 0 0 1 3.5 9h9a1.5 1.5 0 0 1 1.5 1.5V15h.5a.5.5 0 0 0 .5-.5V2.914a.5.5 0 0 0-.146-.353l-1.415-1.415A.5.5 0 0 0 13.086 1H13v4.5A1.5 1.5 0 0 1 11.5 7h-7A1.5 1.5 0 0 1 3 5.5V1H1.5a.5.5 0 0 0-.5.5m3 4a.5.5 0 0 0 .5.5h7a.5.5 0 0 0 .5-.5V1H4zM3 15h10v-4.5a.5.5 0 0 0-.5-.5h-9a.5.5 0 0 0-.5.5z"/></svg>`;
        $('#btnSaveFile').prepend(svgCode);

        svgCode = `<svg width="26" height="26" xmlns="http://www.w3.org/2000/svg" fill="currentColor" class="bi bi-chevron-compact-right" viewBox="0 0 16 16"><path fill-rule="evenodd" d="M6.776 1.553a.5.5 0 0 1 .671.223l3 6a.5.5 0 0 1 0 .448l-3 6a.5.5 0 1 1-.894-.448L9.44 8 6.553 2.224a.5.5 0 0 1 .223-.671"/></svg>`;
        $('#btnGoToListener').prepend(svgCode);
        svgCode = `<svg width="26" height="26" xmlns="http://www.w3.org/2000/svg" fill="currentColor" class="bi bi-chevron-compact-left" viewBox="0 0 16 16"><path fill-rule="evenodd" d="M9.224 1.553a.5.5 0 0 1 .223.67L6.56 8l2.888 5.776a.5.5 0 1 1-.894.448l-3-6a.5.5 0 0 1 0-.448l3-6a.5.5 0 0 1 .67-.223"/></svg>`;
        $('#btnBackToEditor').prepend(svgCode);

        svgCode = `<svg width="26" height="26" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg"><path d="M21.9208 13.265C21.9731 12.8507 22 12.4285 22 12C22 6.47715 17.5228 2 12 2C6.47715 2 2 6.47715 2 12C2 17.5228 6.47715 22 12 22C12.4354 22 12.8643 21.9722 13.285 21.9182M12 6V12L15.7384 13.8692M19 22V16M16 19H22" stroke="#000000" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/></svg>`;
        $('#btnAddDelay').prepend(svgCode);
        svgCode = `<svg width="26" height="26" version="1.1" id="Layer_1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 512.203 512.203" xml:space="preserve"><g><path style="fill:#26B99A;" d="M501.981,236.116l-103.23-74.549c-11.67-8.439-27.992-0.097-27.992,14.31v53.742h-97.103V150.17c0-58.403-47.519-105.931-105.931-105.931h-35.31C121.935,18.375,100.255,0.102,70.621,0.102C31.62,0.102,0,31.722,0,70.722c0,39.009,31.62,70.621,70.621,70.621c29.634,0,51.315-18.264,61.793-44.138h35.31c29.21,0,52.966,23.764,52.966,52.966v79.448h-88.276c-10.478-25.865-32.159-44.138-61.793-44.138c-39,0-70.621,31.62-70.621,70.621c0,39.009,31.62,70.621,70.621,70.621c29.634,0,51.315-18.264,61.793-44.138h88.276v79.448c0,29.21-23.755,52.966-52.966,52.966h-35.31c-10.478-25.865-32.159-44.138-61.793-44.138c-39,0-70.621,31.62-70.621,70.621c0,39.009,31.62,70.621,70.621,70.621c29.634,0,51.315-18.264,61.793-44.138h35.31c58.412,0,105.931-47.519,105.931-105.931v-70.621h97.103v44.924c0,14.398,16.322,22.74,27.992,14.31l103.23-74.549C515.61,266.253,515.61,245.959,501.981,236.116"/></g></svg>`;
        $('#btnMakeFunction').prepend(svgCode);
        svgCode = `<svg width="30" height="30" version="1.1" id="Layer_1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 512.001 512.001" xml:space="preserve"><g><defs><filter id="Adobe_OpacityMaskFilter" filterUnits="userSpaceOnUse" x="115.3" y="0" width="281.401" height="512.001"><feColorMatrix  type="matrix" values="1 0 0 0 0  0 1 0 0 0  0 0 1 0 0  0 0 0 1 0"/></filter></defs><mask maskUnits="userSpaceOnUse" x="115.3" y="0" width="281.401" height="512.001" id="mask-2_1_"><g style="filter:url(#Adobe_OpacityMaskFilter);"><polygon id="path-1_1_" style="fill:#FFFFFF;" points="115.3,511.998 396.701,511.998 396.701,-0.001 115.3,-0.001 			"/></g></mask><path style="mask:url(#mask-2_1_);fill:#57B1E3;" d="M388.955,7.743L388.955,7.743C378.609-2.603,361.819-2.577,351.5,7.796L152.685,210.018c-6.983,7.027-10.902,16.534-10.902,26.439l0,0c0,20.71,16.79,37.199,37.5,37.199H313.85L177.72,412.126l-37.455-37.455L115.3,512.001l137.322-24.973l-37.447-37.447l162.613-164.652c7.018-7.027,10.964-16.552,10.964-26.483l0,0c0-20.683-16.764-37.756-37.447-37.756H215.387L388.955,45.137C399.283,34.808,399.283,18.071,388.955,7.743"/></g></svg>`;
        $('#btnCallFunction').prepend(svgCode);
        svgCode = `<svg width="26" height="26" version="1.1" id="Layer_1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 512 512" xml:space="preserve"><path style="fill:#A9A8AE;" d="M146.45,481.905c0.537,0.538,1.103,1.049,1.69,1.531c-0.582-0.479-1.147-0.99-1.686-1.528L146.45,481.905z"/><path style="fill:#57575C;" d="M272.456,481.91c-4.361,4.361-10.276,6.814-16.446,6.817l232.717-0.002c12.851,0.002,23.273-10.418,23.271-23.271c0-12.854-10.42-23.273-23.271-23.273l-176.542-0.002l-23.273,23.273L272.456,481.91z"/><path style="fill:#D6D5D8;" d="M505.183,216.271l-93.091-93.091L319.001,30.09c-5.112-5.112-12.01-7.35-18.686-6.71c-2.225,0.213-4.425,0.746-6.527,1.598c-0.7,0.284-1.39,0.604-2.065,0.959c-1.351,0.711-2.647,1.562-3.866,2.555c-0.61,0.496-1.199,1.03-1.767,1.598L118.528,197.653L6.819,309.362C2.451,313.726,0.002,319.644,0,325.817c0.002,6.172,2.453,12.091,6.817,16.455l93.091,93.092l46.542,46.541l0.003,0.003c0.54,0.54,1.105,1.05,1.686,1.528c0.141,0.116,0.295,0.214,0.438,0.326c0.458,0.357,0.914,0.711,1.393,1.033c0.133,0.088,0.278,0.164,0.414,0.253c0.507,0.323,1.015,0.641,1.544,0.923c0.098,0.051,0.202,0.095,0.299,0.146c0.576,0.296,1.162,0.582,1.763,0.832c0.064,0.028,0.135,0.048,0.2,0.074c0.635,0.258,1.28,0.493,1.938,0.694c0.073,0.025,0.152,0.037,0.227,0.059c0.65,0.191,1.308,0.362,1.975,0.496c0.188,0.036,0.383,0.053,0.571,0.087c0.559,0.098,1.12,0.194,1.686,0.251c0.763,0.076,1.538,0.116,2.321,0.116l93.091,0c0.005,0,0.008-0.002,0.011,0c6.17-0.003,12.085-2.456,16.446-6.817l16.457-16.457l23.273-23.273l25.434-25.435l167.563-167.562C514.273,240.096,514.273,225.358,505.183,216.271z"/><path style="fill:#DD539C;" d="M505.183,216.271l-93.091-93.091L319.001,30.09c-9.089-9.089-23.823-9.089-32.912,0L118.528,197.653c-4.546,4.544-6.819,10.501-6.819,16.455c0,5.956,2.275,11.912,6.817,16.457l93.091,93.091l76.637,76.635l16.454,16.455c4.546,4.544,10.501,6.817,16.457,6.817c5.955,0,11.911-2.273,16.455-6.819l167.563-167.562C514.273,240.096,514.273,225.358,505.183,216.271z"/><path style="fill:#C3C3C7;" d="M118.526,230.565c-4.543-4.544-6.817-10.501-6.817-16.457c0-5.955,2.273-11.911,6.819-16.455L6.819,309.362C2.451,313.726,0.002,319.645,0,325.817c0.002,6.172,2.453,12.091,6.817,16.455l93.091,93.092l111.709-111.709L118.526,230.565z"/><path style="fill:#DB348A;" d="M118.526,230.565l93.091,93.091l200.475-200.475L319.001,30.09c-9.089-9.089-23.823-9.089-32.912,0L118.528,197.653c-4.546,4.544-6.819,10.501-6.819,16.455C111.709,220.065,113.983,226.021,118.526,230.565z"/></svg>`;
        $('#btnDeleteEvent, #btnDeleteEventAll').prepend(svgCode);
    </script>


    <!-- Scripts -->
    <script src="/DarkJunction/getScripts"></script>

</body>
</html>
)=====";

#include "res_css_DARK_PAGE.h"
#include "res_js_DARK_PAGE.h"