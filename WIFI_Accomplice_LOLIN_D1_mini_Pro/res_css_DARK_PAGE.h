const char raw_css_DARK_PAGE[] PROGMEM =
R"=====(
@import url('https://fonts.cdnfonts.com/css/lcd'); /* https://www.cdnfonts.com/lcd.font */
@font-face {
    font-family: 'D2Coding';
    src: url('https://cdn.jsdelivr.net/gh/projectnoonnu/noonfonts_three@1.0/D2Coding.woff') format('woff');
    font-weight: normal;
    font-style: normal;
}
@font-face {
    font-family: 'LT Binary Neue';
    src: local('LT Binary Neue'), url('https://fonts.cdnfonts.com/s/91304/LTBinaryNeue.woff') format('woff');
    font-weight: 400;
    font-style: normal;
}
@font-face {
    font-family: 'intelone-mono-font-family-regular';
    src: url('https://cdn.jsdelivr.net/gh/projectnoonnu/noonfonts_2307-1@1.1/intelone-mono-font-family-regular.woff2') format('woff2');
    font-weight: 400;
    font-style: normal;
}

.widthMax   { width: 100% }
.heightMax  { height: 100% }
.colorRed   { color: red }
.colorGre   { color: green }
.colorYel   { color: yellow }

html, body {
    height: 100vh;
    margin: 0;
    overflow: hidden;

	font-family: 'D2Coding';
    font-size: 16px;
}

* { scrollbar-color: #2f2f2f rgba(0,0,0,0); /* For, Most modern browsers */ }
*::-webkit-scrollbar-thumb { background-color: #2f2f2f; } /* For, WebKit-based browsers */
*::-webkit-scrollbar { background-color: rgba(0,0,0,0); } /* For, WebKit-based browsers */

.panelLR {
    position: relative;

    outline: 4px solid #2f2f2f;
    border-radius: 18px;
    overflow: hidden;
}

#divTerminal {
    padding: 20px;
    overflow: auto;

    font-family: 'LT Binary Neue';
    background-color: #1a1a1a;
}
#divTerminal p {
    margin: 0;

    color: #fff;
    text-shadow:
        0 0 5px #fff,
        0 0 10px #fff,
        0 0 20px #0ff,
        0 0 40px #0ff,
        0 0 70px #0ff,
        0 0 80px #0ff;
    font-size: 12pt;
}
#divTerminal p a {
    color: #5d5;
    text-shadow: none;
    text-decoration: underline;
}

#tbDirectory tbody {
    display: block;
    overflow-y: scroll;
    overflow-x: hidden;
    max-height: calc(100vh - 4rem - 3rem - 3px); /* 4rem(container-fluid's top bottom padding), 3rem(thead), other(table-group-divider?) */
}
#tbDirectory thead,
#tbDirectory tbody tr{
    display:table;
    table-layout:fixed;
    width:100%;
}
#tbDirectory th,
#tbDirectory td {
    overflow: hidden;
    white-space: nowrap; /* disable automatic line wrapping */
    text-overflow: ellipsis; /* overflowed texts convert '...' */

    color: #ECECEC;
}
#tbDirectory th {
    padding-left: 2.0vw;
    padding-right: 2.0vw;
    padding-bottom: 0.4rem;

    cursor: default;
    font-size: 24px;
}
#tbDirectory td {
    padding-left: 2.0vw;
    padding-right: calc(2.0vw - 15px);
    padding-top: 0.5rem;
    padding-bottom: 0.4rem;

    font-family: 'intelone-mono-font-family-regular';
}
#tbDirectory th:nth-child(2),
#tbDirectory td:nth-child(2) {
    text-align: right;
}
.table tbody tr:last-child {
    border-bottom: hidden;
}
.gradient-animation-text:hover {
    cursor: pointer;
    color: transparent;
    background-size: 0% 3px;
    background-image: linear-gradient(90deg, red,orange,green,cyan,purple,cyan,green,orange,red);
    background-clip: text;
    background-size: 500% 100px;
    animation: animate-wave 15s linear infinite;
    transition: background-size 500ms ease;
}
@keyframes animate-wave { 0% { background-position-x: 0; } 100% { background-position-x: -1600px; } }

#taFileContent {
    padding: 20px;
    resize: none; /* hide resizer */
    border: none;
    outline: none;
    overflow: auto;
    white-space: nowrap; /* disable automatic line wrapping */

    background-image: url('https://i.imgur.com/DN9oKBA.jpg');
    background-repeat: no-repeat;
    background-size: contain;
    background-position: center;
}

#divFileSelector {
    display: none;
    position: absolute;
    top: 0px;
    right: 0px;
    height: 22px;
    margin-top: -4px;
    padding-left: 25px;
    border-bottom-left-radius: 18px;

    background-color: #2f2f2f;
}

#divFileSelector span {
    padding-right: 20px;

    cursor: pointer;
    color: #2F2F2F;
    text-shadow:
        0 0 5px #fff,
        0 0 10px #fff;
    font-family: 'LCDMono2';
}
#divFileSelector .fileSelected {
    cursor: default;
    color: #ECECEC;
}

.btnTextareaRB {
    position: absolute;
    right: 1.5rem;
    bottom: 1.5rem;

    width: 2.5rem;
    height: 2.5rem;
    border: none;
    border-radius: 50%;
    outline: 4px solid #2f2f2f;

    cursor: pointer;
    color: #ECECEC;
    font-size: 40px;
    line-height: 5px;
    transition: all 0.4s ease;
}
.btnTextareaRB-Dark { background-color: #212121; }
.btnTextareaRB-Dark:hover { background-color: #4f4f4f; }
.btnTextareaRB-Gray { background-color: #4f4f4f; }
.btnTextareaRB-Gray:hover { background-color: #212121; }

/* Split.js */
.split {
    display: flex;
    flex-direction: row;
}
.gutter {
    background-color: rgba(0,0,0,0);
    background-repeat: no-repeat;
    background-position: 50%;
}
.gutter.gutter-horizontal {
    cursor: col-resize;
    /*background-image: url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAUAAAAeCAYAAADkftS9AAAAIklEQVQoU2M4c+bMfxAGAgYYmwGrIIiDjrELjpo5aiZeMwF+yNnOs5KSvgAAAABJRU5ErkJggg==');*/
}

/* sweetalert2 */
.swal2-container button {
    margin: 0 1rem;
    width: 5rem;
    height: 3rem;
}

/* contextMenu.min.js (declared in js) */
.contextMenu {
    outline: 4px solid #2f2f2f;
    border-radius: 5px;

    background-color: #4f4f4f;
}
.contextMenuItem {
    display: flex;
    padding: 0.4rem;
    padding-bottom: 0.1rem;
    border-radius: 3px;

    justify-content: center;
    align-items: center;
    align-content: center;
    color: #ECECEC;
    transition: all 0.4s ease;
    font-family: 'LT Binary Neue';
}
.contextMenuItem:hover {
    background-color: #212121;
}
)=====";