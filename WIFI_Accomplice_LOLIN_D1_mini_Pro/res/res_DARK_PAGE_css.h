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
    src: url('https://fonts.cdnfonts.com/s/91304/LTBinaryNeue.woff') format('woff');
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
.colorRed   { color: red !important; }
.colorGre   { color: green !important; }
.colorYel   { color: yellow !important; }

html, body {
    height: 100vh;
    margin: 0;
    padding: 0;
    overflow: hidden;
    touch-action: none;

	font-family: 'D2Coding';
    font-size: 16px;
}

* { scrollbar-color: #2f2f2f transparent; /* For, Most modern browsers */ }
*::-webkit-scrollbar-thumb { background-color: #2f2f2f; } /* For, WebKit-based browsers */
*::-webkit-scrollbar { background-color: transparent; } /* For, WebKit-based browsers */

.panelLR {
    position: relative;

    outline: 4px solid #2f2f2f;
    border-radius: 18px;
    overflow: hidden;
}
.panelListener {
    position: relative;

    outline: 4px solid #2f2f2f;
    border-radius: 5px;
    overflow-x: hidden;
    overflow-y: auto;

    background-color: rgba(100, 100, 100, 0.1);

    backdrop-filter: blur(4px);
    -webkit-backdrop-filter: blur(4px);
}
.panelListener > * {
    margin-top: 2rem;
}

#divTerminal {
    padding: 20px;
    overflow: auto;

    font-family: 'LT Binary Neue';
    background-color: #171717;
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
.rowMAPPER { color: orange; }
.rainbow-text-when-hover:hover {
    cursor: pointer;
    color: transparent;
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
    overflow-y: scroll;
    overflow-x: auto;
    white-space: nowrap; /* disable automatic line wrapping */

    background-image: url('https://i.imgur.com/DN9oKBA.jpg');
    background-repeat: no-repeat;
    background-size: contain;
    background-position: center;
}

#divFileList {
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

#divFileList span {
    padding-right: 20px;

    cursor: pointer;
    color: #2F2F2F;
    text-shadow:
        0 0 5px #fff,
        0 0 10px #fff;
    font-family: 'LCDMono2';
}
#divFileList .fileSelected {
    cursor: default;
    color: #ECECEC;
}

.btnTogglePage {
    --btnTogglePage-height: 6rem;
    --btnTogglePage-width: 2.5rem;

    position: absolute;
    top: calc(50% - 0.5* var(--btnTogglePage-height));
    padding: 2px;
    z-index: 999;
    overflow: hidden;

    height: var(--btnTogglePage-height);
    width: var(--btnTogglePage-width);
    border: none;
    color: #ECECEC;
    background-color: #2f2f2f;
}
.btnTogglePage-R {
    right: -1rem;

    border-top-left-radius: 2rem 4rem;
    border-bottom-left-radius: 2rem 4rem;
    text-align: left;
}
.btnTogglePage-L {
    left: -1rem;

    border-top-right-radius: 2rem 4rem;
    border-bottom-right-radius: 2rem 4rem;
    text-align: right;
}

.btnBottomRight {
    position: absolute;
    right: 3.5rem;
    bottom: 3.5rem;

    width: 2.5rem;
    height: 2.5rem;
    border: none;
    border-radius: 50%;
    outline: 2px solid #ECECEC;

    cursor: pointer;
    color: #ECECEC;
    font-size: 40px;
    line-height: 5px;
    transition: all 0.4s ease;
}
#btnCreateFile { background-color: rgb(0,175,0); }
#btnCreateFile:hover { background-color: rgb(0,100,0); }
#btnSaveFile { background-color: rgb(255,75,25); }
#btnSaveFile:hover { background-color: rgb(125,0,0); }
#btnHelp { background-color: rgb(0,120,200); }
#btnHelp:hover { background-color: rgb(0,45,150); }



.selectList {
    width: 100%;
    height: 100%;
    padding: .5rem;
    border-radius: 5px;
    outline: none;

    -webkit-appearance: none;
    -moz-appearance: none;
    appearance: none;
}
.selectList option{
	height: 1.5rem;
    display: flex;
    align-items: center;
}
#divEventModifier {
    position: relative;
    width: 100px;
    margin: 0;
    padding: 0;
}
#divEventModifier > * {
	margin: 0 auto 1.5rem auto;
    padding: 0;
    border: none;
}
#divEventModifier > :last-child {
    margin-bottom: 0;
}
#divEventModifier input {
	width: 6rem;
	height: 1.5rem;
    padding: 0;
    border-radius: 15px;
    font-size: .9rem;
	text-align: center;

    -moz-appearance: textfield;
}
#divEventModifier input::placeholder {
    text-align: center;
    font-size: .75rem;
}
#divEventModifier .btnEventModifier {
    width: 3rem;
    height: 3rem;
    border-radius: 100%;
}



/* pagePiling.js */
html,body{overflow:hidden;margin:0;padding:0;-webkit-tap-highlight-color:transparent}.pp-section{height:100%;position:absolute;width:100%}.pp-easing{-webkit-transition:all 1000ms cubic-bezier(.55,.085,0,.99);-moz-transition:all 1000ms cubic-bezier(.55,.085,0,.99);-o-transition:all 1000ms cubic-bezier(.55,.085,0,.99);transition:all 1000ms cubic-bezier(.55,.085,0,.99);-webkit-transition-timing-function:cubic-bezier(.55,.085,0,.99);-moz-transition-timing-function:cubic-bezier(.55,.085,0,.99);-o-transition-timing-function:cubic-bezier(.55,.085,0,.99);transition-timing-function:cubic-bezier(.55,.085,0,.99)}#pp-nav{position:fixed;z-index:100;margin-top:-32px;top:50%;opacity:1}#pp-nav.right{right:17px}#pp-nav.left{left:17px}.pp-section.pp-table{display:table}.pp-tableCell{display:table-cell;vertical-align:middle;width:100%;height:100%}.pp-slidesNav{position:absolute;z-index:4;left:50%;opacity:1}.pp-slidesNav.bottom{bottom:17px}.pp-slidesNav.top{top:17px}#pp-nav ul,.pp-slidesNav ul{margin:0;padding:0}#pp-nav li,.pp-slidesNav li{display:block;width:14px;height:13px;margin:7px;position:relative}.pp-slidesNav li{display:inline-block}#pp-nav li a,.pp-slidesNav li a{display:block;position:relative;z-index:1;width:100%;height:100%;cursor:pointer;text-decoration:none}#pp-nav li .active span,.pp-slidesNav .active span{background:#333}#pp-nav span,.pp-slidesNav span{top:2px;left:2px;width:8px;height:8px;border:1px solid #000;background:transparent;border-radius:50%;position:absolute;z-index:1}.pp-tooltip{position:absolute;top:-2px;color:#fff;font-size:14px;font-family:arial,helvetica,sans-serif;white-space:nowrap;max-width:220px}.pp-tooltip.right{right:20px}.pp-tooltip.left{left:20px}.pp-scrollable{overflow-y:scroll;height:100%}

/* Split.js */
.split {
    display: flex;
    flex-direction: row;
}
.gutter {
    background-color: transparent;
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

/* context-js (https://github.com/heapoverride/context-js) */
.context {
    display: inline-block;
    position: fixed;
    top: 0px;
    left: 0px;
    min-width: auto;
    font-family: 'LT Binary Neue';
    color: #fff;
    background: #262933;
    font-size: 10pt;
    border: 1px solid #333333;
    border-radius: 6px;
    box-shadow: 2px 2px 2px -1px rgba(0, 0, 0, 0.5);
    padding: 3px 0px;
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
}
.context .item {
    padding: 4px 10px;
    cursor: default;
    color: inherit;
}
.context .item:hover {
    background: #2777FF;
}
.context .item:hover .hotkey {
    color: #fff;
}
.context .disabled {
    color: #878B90;
}
.context .disabled:hover {
    background: inherit;
}
.context .disabled:hover .hotkey {
    color: #878B90;
}
.context .separator {
    margin: 4px 0px;
    height: 0;
    padding: 0;
    border-top: 1px solid #454545;
}
.hotkey {
    color: #878B90;
    float: right;
}

/* Keypress: A robust Javascript library for capturing keyboard input */
:root {
	--keyboard-key-font-size:12px;
	--keyboard-key-single-font-size:12px;
	--keyboard-key-span-font-size:10px;
	--keyboard-key-pressed-color:#cf3ef0
}
.keyboard {
	padding:1px;
	zoom:1
}
.keyboard:before,.keyboard:after {
	content:"";
	display:table
}
.keyboard:after {
	clear:both
}
.keyboard div {
	padding:0
}
.keyboard section {
	float:left;
	margin:0 20px
}
.keyboard section:first-child .key_filler {
	pointer-events:none;
	width:18px
}
.keyboard section .row>:last-child {
	margin-right:0
}
.keyboard .row {
	height:40px;
	margin-bottom:5px
}
.keyboard section>:last-child {
	margin-bottom:0
}
.keyboard .key,.keyboard .key_filler {
	display:inline-block;
	position:relative;
	margin-right:5px;
	width:40px;
	height:40px
}
.keyboard .key_filler {
	pointer-events:none
}
.keyboard .key {
	outline:.5px solid #2f2f2f;
	padding-top:.25em;
	vertical-align:top;
	font-size:var(--keyboard-key-font-size);
	color:#333;
	text-align:center;
	-webkit-border-radius:5px;
	-moz-border-radius:5px;
	border-radius:5px;
	background:#fff;
	background-image:-moz-linear-gradient(-90deg,white 0,#e3e3e3 100%);
	background-image:-webkit-linear-gradient(-90deg,white 0,#e3e3e3 100%);
	background-image:-o-linear-gradient(-90deg,white 0,#e3e3e3 100%);
	background-image:-ms-linear-gradient(-90deg,white 0,#e3e3e3 100%)
}
.keyboard .key.pressed,.keyboard .key.shift_pressed {
	outline-offset:-2px;
	outline:4px solid var(--keyboard-key-pressed-color);
	color:var(--keyboard-key-pressed-color);
	background:#f3f3f3
}
.keyboard .key.pressed strong,.keyboard .key.shift_pressed strong {
	color:var(--keyboard-key-pressed-color)
}
.keyboard .key.pressed .triangle.up,.keyboard .key.shift_pressed .triangle.up {
	border-color:transparent transparent var(--keyboard-key-pressed-color)
}
.keyboard .key.pressed .triangle.down,.keyboard .key.shift_pressed .triangle.down {
	border-color:var(--keyboard-key-pressed-color) transparent transparent
}
.keyboard .key.pressed .triangle.right,.keyboard .key.shift_pressed .triangle.right {
	border-color:transparent transparent transparent var(--keyboard-key-pressed-color)
}
.keyboard .key.pressed .triangle.left,.keyboard .key.shift_pressed .triangle.left {
	border-color:transparent var(--keyboard-key-pressed-color) transparent transparent
}
.keyboard .key.shift_pressed strong {
	color:#333
}
.keyboard .key.shift_pressed em {
	color:var(--keyboard-key-pressed-color)
}
.keyboard .key.wide_1 {
	width:61px
}
.keyboard .key.wide_2 {
	width:64px
}
.keyboard .key.wide_3 {
	width:74.5px
}
.keyboard .key.wide_4 {
	width:97px
}
.keyboard .key.wide_5 {
	width:253px
}
.keyboard .key.wide_6 {
	width:85px
}
.keyboard .key.tall {
	height:85px;
	line-height:85px
}
.keyboard .key.single {
	font-size:var(--keyboard-key-single-font-size);
	padding-top:.35em
}
.keyboard .key em {
	font-style:normal
}
.keyboard .key strong {
	font-weight:400
}
.keyboard .key .triangle {
	margin:8px auto
}
.keyboard .key .triangle.up {
	top:2px
}
.keyboard .key .triangle.down {
	top:5px
}
.keyboard .key .triangle.left {
	left:-2px
}
.keyboard .key .triangle.right {
	left:2px
}
.keyboard .key br+.triangle {
	margin:0 auto
}
.keyboard .key span {
	display:block;
	position:absolute;
	width:100%;
	padding:.2em .5em;
	bottom:0;
	font-size:var(--keyboard-key-span-font-size);
	text-align:center
}
.keyboard .key span.right {
	text-align:right
}
.keyboard .key span.left {
	text-align:left
}
.keyboard .triangle {
	position:relative;
	width:0;
	height:0;
	border-style:solid
}
.keyboard .triangle.up {
	border-width:0 8px 10px;
	border-color:transparent transparent #555
}
.keyboard .triangle.down {
	border-width:10px 8px 0;
	border-color:#555 transparent transparent
}
.keyboard .triangle.right {
	border-width:8px 0 8px 10px;
	border-color:transparent transparent transparent #555
}
.keyboard .triangle.left {
	border-width:8px 8px 10px 0;
	border-color:transparent #555 transparent transparent
}
)=====";