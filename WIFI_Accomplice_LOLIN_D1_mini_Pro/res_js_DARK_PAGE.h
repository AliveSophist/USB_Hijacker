const char raw_js_DARK_PAGE[] PROGMEM =
R"=====(
/**⦓ SCRIPT, ABOUT LIBRARIES ⦔**/

// pagePiling.js
(function(a,e,d,c){a.fn.pagepiling=function(Y){function W(f){var k=a(".pp-section.active").index(".pp-section");f=f.index(".pp-section");return k>f?"up":"down"}function am(f,k){var l={destination:f,animated:k,activeSection:a(".pp-section.active"),anchorLink:f.data("anchor"),sectionIndex:f.index(".pp-section"),toMove:f,yMovement:W(f),leavingSection:a(".pp-section.active").index(".pp-section")+1};l.activeSection.is(f)||("undefined"===typeof l.animated&&(l.animated=!0),"undefined"!==typeof l.anchorLink&&T(l.anchorLink,l.sectionIndex),l.destination.addClass("active").siblings().removeClass("active"),l.sectionsToMove=R(l),"down"===l.yMovement?(l.translate3d="vertical"!==options.direction?"translate3d(-100%, 0px, 0px)":"translate3d(0px, -100%, 0px)",l.scrolling="-100%",options.css3||l.sectionsToMove.each(function(m){m!=l.activeSection.index(".pp-section")&&a(this).css(ai(l.scrolling))}),l.animateSection=l.activeSection):(l.translate3d="translate3d(0px, 0px, 0px)",l.scrolling="0",l.animateSection=f),a.isFunction(options.onLeave)&&options.onLeave.call(this,l.leavingSection,l.sectionIndex+1,l.yMovement),s(l),o(l.anchorLink),j(l.anchorLink,l.sectionIndex),X=l.anchorLink,V=(new Date).getTime())}function s(f){options.css3?(U(f.animateSection,f.translate3d,f.animated),f.sectionsToMove.each(function(){U(a(this),f.translate3d,f.animated)}),setTimeout(function(){ag(f)},options.scrollingSpeed)):(f.scrollOptions=ai(f.scrolling),f.animated?f.animateSection.animate(f.scrollOptions,options.scrollingSpeed,options.easing,function(){S(f);ag(f)}):(f.animateSection.css(ai(f.scrolling)),setTimeout(function(){S(f);ag(f)},400)))}function ag(f){a.isFunction(options.afterLoad)&&options.afterLoad.call(this,f.anchorLink,f.sectionIndex+1)}function R(f){return"down"===f.yMovement?a(".pp-section").map(function(k){if (k<f.destination.index(".pp-section")){return a(this)}}):a(".pp-section").map(function(k){if (k>f.destination.index(".pp-section")){return a(this)}})}function S(f){"up"===f.yMovement&&f.sectionsToMove.each(function(k){a(this).css(ai(f.scrolling))})}function ai(f){return"vertical"===options.direction?{top:f}:{left:f}}function T(k,f){options.anchors.length?(location.hash=k,u(location.hash)):u(String(f))}function u(f){f=f.replace("#","");a("body")[0].className=a("body")[0].className.replace(/\b\s?pp-viewing-[^\s]+\b/g,"");a("body").addClass("pp-viewing-"+f)}function ae(){return(new Date).getTime()-V<600+options.scrollingSpeed?!0:!1}function U(k,f,l){k.toggleClass("pp-easing",l);k.css({"-webkit-transform":f,"-moz-transform":f,"-ms-transform":f,transform:f})}function al(f){if (!ae()){f=d.event||f;f=Math.max(-1,Math.min(1,f.wheelDelta||-f.deltaY||-f.detail));var k=a(".pp-section.active"),k=ah(k);0>f?ak("down",k):ak("up",k);return !1}}function ak(l,k){if ("down"==l){var n="bottom",m=ao.moveSectionDown}else{n="top",m=ao.moveSectionUp}if (0<k.length){if (n="top"===n?!k.scrollTop():"bottom"===n?k.scrollTop()+1+k.innerHeight()>=k[0].scrollHeight:void 0,n){m()}else{return !0}}else{m()}}function ah(f){return scrollable=f.filter(".pp-scrollable")}function af(){return d.PointerEvent?{down:"pointerdown",move:"pointermove",up:"pointerup"}:{down:"MSPointerDown",move:"MSPointerMove",up:"MSPointerUp"}}function ad(k){var f=[];f.y="undefined"!==typeof k.pageY&&(k.pageY||k.pageX)?k.pageY:k.touches[0].pageY;f.x="undefined"!==typeof k.pageX&&(k.pageY||k.pageX)?k.pageX:k.touches[0].pageX;return f}function ac(f){return"undefined"===typeof f.pointerType||"mouse"!=f.pointerType}function i(f){f=f.originalEvent;ac(f)&&(f=ad(f),aj=f.y,touchStartX=f.x)}function h(f){var k=f.originalEvent;!ab(f.target)&&ac(k)&&(f.preventDefault(),f=a(".pp-section.active"),f=ah(f),ae()||(k=ad(k),touchEndY=k.y,touchEndX=k.x,"horizontal"===options.direction&&Math.abs(touchStartX-touchEndX)>Math.abs(aj-touchEndY)?Math.abs(touchStartX-touchEndX)>an.width()/100*options.touchSensitivity&&(touchStartX>touchEndX?ak("down",f):touchEndX>touchStartX&&ak("up",f)):Math.abs(aj-touchEndY)>an.height()/100*options.touchSensitivity&&(aj>touchEndY?ak("down",f):touchEndY>aj&&ak("up",f))))}function ab(f,k){k=k||0;var l=a(f).parent();return k<options.normalScrollElementTouchThreshold&&l.is(options.normalScrollElements)?!0:k==options.normalScrollElementTouchThreshold?!1:ab(l,++k)}function g(){a("body").append('<div id="pp-nav"><ul></ul></div>');var f=a("#pp-nav");f.css("color",options.navigation.textColor);f.addClass(options.navigation.position);for(var l=0;l<a(".pp-section").length;l++){var m="";options.anchors.length&&(m=options.anchors[l]);if ("undefined"!==options.navigation.tooltips){var k=options.navigation.tooltips[l];"undefined"===typeof k&&(k="")}f.find("ul").append('<li data-tooltip="'+k+'"><a href="#'+m+'"><span></span></a></li>')}f.find("span").css("border-color",options.navigation.bulletsColor)}function j(f,k){options.navigation&&(a("#pp-nav").find(".active").removeClass("active"),f?a("#pp-nav").find('a[href="#'+f+'"]').addClass("active"):a("#pp-nav").find("li").eq(k).find("a").addClass("active"))}function o(f){options.menu&&(a(options.menu).find(".active").removeClass("active"),a(options.menu).find('[data-menuanchor="'+f+'"]').addClass("active"))}function b(){var k=e.createElement("p"),f,m={webkitTransform:"-webkit-transform",OTransform:"-o-transform",msTransform:"-ms-transform",MozTransform:"-moz-transform",transform:"transform"};e.body.insertBefore(k,null);for(var l in m){k.style[l]!==c&&(k.style[l]="translate3d(1px,1px,1px)",f=d.getComputedStyle(k).getPropertyValue(m[l]))}e.body.removeChild(k);return f!==c&&0<f.length&&"none"!==f}var ao=a.fn.pagepiling,an=a(this),X,V=0,Z="ontouchstart" in d||0<navigator.msMaxTouchPoints||navigator.maxTouchPoints,aj=touchStartX=touchEndY=touchEndX=0;options=a.extend(!0,{direction:"vertical",menu:null,verticalCentered:!0,sectionsColor:[],anchors:[],scrollingSpeed:700,easing:"easeInQuart",loopBottom:!1,loopTop:!1,css3:!0,navigation:{textColor:"#000",bulletsColor:"#000",position:"right",tooltips:[]},normalScrollElements:null,normalScrollElementTouchThreshold:5,touchSensitivity:5,sectionSelector:".section",animateAnchor:!1,afterLoad:null,onLeave:null,afterRender:null},Y);a.extend(a.easing,{easeInQuart:function(l,k,p,n,m){return n*(k/=m)*k*k*k+p}});ao.setScrollingSpeed=function(f){options.scrollingSpeed=f};ao.moveSectionUp=function(){var f=a(".pp-section.active").prev(".pp-section");!f.length&&options.loopTop&&(f=a(".pp-section").last());f.length&&am(f)};ao.moveSectionDown=function(){var f=a(".pp-section.active").next(".pp-section");!f.length&&options.loopBottom&&(f=a(".pp-section").first());f.length&&am(f)};ao.moveTo=function(f){var k="",k=isNaN(f)?a('[data-anchor="'+f+'"]'):a(".pp-section").eq(f-1);0<k.length&&am(k)};a(options.sectionSelector).each(function(){a(this).addClass("pp-section")});options.css3&&(options.css3=b());a(an).css({overflow:"hidden","-ms-touch-action":"none","touch-action":"none"});a.isEmptyObject(options.navigation)||g();var aa=a(".pp-section").length;a(".pp-section").each(function(f){a(this).data("data-index",f);a(this).css("z-index",aa);f||0!==a(".pp-section.active").length||a(this).addClass("active");"undefined"!==typeof options.anchors[f]&&a(this).attr("data-anchor",options.anchors[f]);"undefined"!==typeof options.sectionsColor[f]&&a(this).css("background-color",options.sectionsColor[f]);options.verticalCentered&&a(this).addClass("pp-table").wrapInner('<div class="pp-tableCell" style="height:100%" />');--aa}).promise().done(function(){options.navigation&&(a("#pp-nav").css("margin-top","-"+a("#pp-nav").height()/2+"px"),a("#pp-nav").find("li").eq(a(".pp-section.active").index(".pp-section")).find("a").addClass("active"));a(d).on("load",function(){var f=d.location.hash.replace("#",""),f=a('.pp-section[data-anchor="'+f+'"]');0<f.length&&am(f,options.animateAnchor)});a.isFunction(options.afterRender)&&options.afterRender.call(this)});a(d).on("hashchange",function(){var f=d.location.hash.replace("#","").split("/")[0];f.length&&f&&f!==X&&(f=isNaN(f)?a('[data-anchor="'+f+'"]'):a(".pp-section").eq(f-1),am(f))});a(e).on("click touchstart","#pp-nav a",function(f){f.preventDefault();f=a(this).parent().index();am(a(".pp-section").eq(f))});a(e).on({mouseenter:function(){var f=a(this).data("tooltip");a('<div class="pp-tooltip '+options.navigation.position+'">'+f+"</div>").hide().appendTo(a(this)).fadeIn(200)},mouseleave:function(){a(this).find(".pp-tooltip").fadeOut(200,function(){a(this).remove()})}},"#pp-nav li")}})(jQuery,document,window);
const SECTION_EDITOR    = 0;
const SECTION_LISTENER  = 1;
var nowSection = SECTION_EDITOR;
function initializePagePiling()
{
    $('#pagepiling').pagepiling(
    {
        direction: 'horizontal',
        anchors: ['pageEditor', 'pageListener'],
        navigation: false
    });

    window.addEventListener("hashchange", function()
    {
        // SECTION_EDITOR to SECTION_LISTENER
        if (nowSection==SECTION_EDITOR && window.location.hash==="#pageListener")
        {
            nowSection = SECTION_LISTENER;
            resetListener();

            if (taFileContent.dataset.currentFilename.length < 1)
                return;


            const extractMacroFuncs = (arrLines) =>
            {
                while(true)
                {
                    let arrLeftLines = [];
                    let strMerge = '';

                    let funcName = null;
                    let funcValue = null;

                    // find func
                    for(strLine of arrLines)
                    {
                        if (funcName == null)
                        {
                            let pattern = /^[@].*?(?=\s|\0|$)/;
                            if (pattern.test(strLine))
                                funcName = pattern.exec(strLine)[0].slice(1);
                        }
                        else
                        if (funcValue == null)
                        {
                            strMerge += strLine + '\n';

                            if (strLine.includes('}'))
                                funcValue = strMerge;
                        }
                        else
                            arrLeftLines.push(strLine);
                    }
                    if (funcName == null)
                        break;

                    // if func exists, create element and loop again
                    createOptionToFunctionList("@"+funcName, funcName, funcValue)
                    arrLines = arrLeftLines;
                }

                return arrLines;
            }
            const analyzeMacroEvent = (strLine) =>
            {
                let pattern;

                // [DN KEY_X], [DNUP KEY_X], [UP 0x74] ...
                pattern = /^(DN|UP).{1,4}?(KEY|0x).*?(?=\s|\0|$)/;
                if (pattern.test(strLine))
                    return pattern.exec(strLine)[0];

                // [TY "xxxxxxxxx"]
                pattern = /^(TY)/;
                if (pattern.test(strLine)) {
                    let indexStart = strLine.indexOf("\"");
                    let indexEnd = strLine.lastIndexOf("\"");

                    if (indexStart != -1 && indexStart < indexEnd)
                        return `TY ("...")`;
                }

                // [$calledFuncName]
                pattern = /^[$].*?(?=\s|\0|$)/;
                if (pattern.test(strLine))
                    return pattern.exec(strLine)[0];

                // [1234], [123 ~ 1234]
                pattern = /\d+/;
                if (pattern.test(strLine)) {
                    if (strLine.includes('~'))
                        return pattern.exec(strLine)[0] + "~" + pattern.exec( strLine.slice(strLine.indexOf('~')+1) )[0] + "ms";
                    else
                        return pattern.exec(strLine)[0] + "ms";
                }

                // commented line & spaced line
                pattern = /^(\s*\/{2})/;
                if (pattern.test(strLine) || strLine === '')
                    return '';

                // unanalyzable line
                return undefined;
            }


            // Zero, keycode to readable keyStr
            let arrLines = taFileContent.value.split('\n');
            for(let i = 0; i < arrLines.length; i++) {
                if (arrLines[i].includes('0x'))
                    arrLines[i] = arrLines[i].replace(/0x[0-9A-Fa-f]{2}/g, (match) => { return keycode_To_keyStr(match.toUpperCase()); });
            }

            // First, extract Funcs
            arrLines = extractMacroFuncs(arrLines);

            // Second, tidy up top & bottom things
            while(true) {
                if (arrLines[0] == '')
                    arrLines.shift();
                else
                    break;
            }
            while(true) {
                if (arrLines[arrLines.length-1] == '')
                    arrLines.pop();
                else
                    break;
            }

            // Third, organize EventList
            for(strLine of arrLines)
            {
                let strAnalyzed = analyzeMacroEvent(strLine);
                if (strAnalyzed == undefined)
                    continue;
                if (strLine.includes("//"))
                    strAnalyzed += (strAnalyzed.length>0 ? " " : '') + "//...";

                createOptionToEventList(strAnalyzed, strLine);
            }
        }
        else
        // SECTION_LISTENER to SECTION_EDITOR
        if (nowSection==SECTION_LISTENER && window.location.hash==="#pageEditor")
        {
            nowSection = SECTION_EDITOR;

            if (taFileContent.dataset.currentFilename.length < 1)
                return;


            let strCompress = '';
            for(op of selectFunctionList.querySelectorAll('option'))
                strCompress += "@" + op.dataset.funcName + "\n" + op.dataset.funcValue;
            if (selectFunctionList.innerHTML != '')
                strCompress += "\n";
            for(op of selectEventList.querySelectorAll('option'))
                strCompress += op.dataset.eventDetail + "\n";

            // taFileContent.value = strCompress;

            let arrLines = strCompress.split('\n');
            for(let i = 0; i < arrLines.length; i++) {
                if (arrLines[i].includes('KEY_'))
                    arrLines[i] = arrLines[i].replace(/KEY_\S+(?=\s|\0|$)/g, (match) => { return keyStr_To_keycode(match); });
                else
                if (arrLines[i].includes('KEYPAD_'))
                    arrLines[i] = arrLines[i].replace(/KEYPAD_\S+(?=\s|\0|$)/g, (match) => { return keyStr_To_keycode(match); });
            }

            taFileContent.value = arrLines.join('\n');
        }
    });
}

// Split.js configuration
function initializeSplit()
{
    document.querySelector("#divPanelL").parentNode.classList.add('split');
    Split(['#divPanelL', '#divPanelR'], { minSize: 0, gutterSize: 18, });
}

// sweetalert2
const CommonSwal = Swal.mixin({ heightAuto: false });
const DialogSwal = CommonSwal.mixin(
{
    position: "top-end",
    showConfirmButton: false,
    timer: 1500
});
const PromptSwal = CommonSwal.mixin(
{
	showCancelButton: true,
    buttonsStyling: false,
    customClass: {
        cancelButton: "btn btn-outline-secondary",
        confirmButton: "btn btn-outline-primary"
    },
    cancelButtonText: `<svg xmlns="http://www.w3.org/2000/svg" width="25" height="25" fill="currentColor" class="bi bi-x-lg" viewBox="0 0 16 16"><path d="M2.146 2.854a.5.5 0 1 1 .708-.708L8 7.293l5.146-5.147a.5.5 0 0 1 .708.708L8.707 8l5.147 5.146a.5.5 0 0 1-.708.708L8 8.707l-5.146 5.147a.5.5 0 0 1-.708-.708L7.293 8z"/></svg>`,
    confirmButtonText: `<svg xmlns="http://www.w3.org/2000/svg" width="25" height="25" fill="currentColor" class="bi bi-check-lg" viewBox="0 0 16 16"><path d="M12.736 3.97a.733.733 0 0 1 1.047 0c.286.289.29.756.01 1.05L7.88 12.01a.733.733 0 0 1-1.065.02L3.217 8.384a.757.757 0 0 1 0-1.06.733.733 0 0 1 1.047 0l3.052 3.093 5.4-6.425z"/></svg>`,
    reverseButtons: true
});

// DataTables
let dtDirectory;
let filenameLatestRightClicked;
function initializeDataTable()
{
    dtDirectory = $('#tbDirectory').DataTable(
    {
        lengthChange: false,
        searching: false,
        info: false,
        paging: false,
        select: false,

        columnDefs:
        [
            {	targets: 0,
                type: 'string',
                render: function(data, type, row, meta)
                {
                    if (type === 'display')
                        return `<span class="btnPullFile rainbow-text-when-hover">${data}</span>`;
                    else
                        return data;
                }
            },
            {	targets: 1,
                type: 'num',
                render: function(data, type, row)
                {
                    if (type === 'display')
                        return data + " Bytes";
                    else
                        return data;
                }
            }
        ],
        ordering: true
    });
    dtDirectory.on('order.dt', function()
    {
        const rowMAPPER = $("#tbDirectory tbody").find("tr:contains('MAPPER.TXT')");
        const rowKEYCODE = $("#tbDirectory tbody").find("tr:contains('KEYCODE.TXT')");

        if (rowKEYCODE) $("#tbDirectory tbody").prepend( rowKEYCODE );   // always 2nd row
        if (rowMAPPER) $("#tbDirectory tbody").prepend( rowMAPPER );     // always 1st row
    });
    dtDirectory.order([0, 'asc']).draw();
}
function findRow(filename)
{
    return [...document.querySelectorAll("#tbDirectory tr")].find(row => row.textContent.includes(filename));
}
function getRowData(filename)
{
    return dtDirectory.row(findRow(filename)).data();
}
function createRow(filename, filesize)
{
    dtDirectory.row.add([ filename, filesize ]).draw();

    const rowNew = findRow(filename);
    rowNew.dataset.filename = filename;
    rowNew.dataset.filesize = filesize;
    rowNew.querySelector(".btnPullFile").addEventListener("click", function()
    {   executePullFile( filename );   });
    rowNew.addEventListener("contextmenu", function()
    {   filenameLatestRightClicked = filename;   });
}
function deleteRow(filename)
{
    dtDirectory.row(findRow(filename)).remove().draw();
}

// context-js (https://github.com/heapoverride/context-js)
class ContextMenu{constructor(t,e){this.container=t,this.dom=null,this.shown=!1,this.root=!0,this.parent=null,this.submenus=[],this.items=e,this._onclick=(t=>{!this.dom||t.target==this.dom||t.target.parentElement==this.dom||t.target.classList.contains("item")||t.target.parentElement.classList.contains("item")||this.hideAll()}),this._oncontextmenu=(t=>{t.preventDefault(),t.target==this.dom||t.target.parentElement==this.dom||t.target.classList.contains("item")||t.target.parentElement.classList.contains("item")||(this.hideAll(),this.show(t.clientX,t.clientY))}),this._oncontextmenu_keydown=(t=>{93==t.keyCode&&(t.preventDefault(),this.hideAll(),this.show(t.clientX,t.clientY))}),this._onblur=(t=>{this.hideAll()})}getMenuDom(){const t=document.createElement("div");t.classList.add("context");for(const e of this.items)t.appendChild(this.itemToDomEl(e));return t}itemToDomEl(t){const e=document.createElement("div");if (null===t)return e.classList="separator",e;t.hasOwnProperty("color")&&/^#([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(t.color.toString())&&(e.style.cssText=`color: ${t.color}`),e.classList.add("item");const s=document.createElement("span");s.classList="label",s.innerText=t.hasOwnProperty("text")?t.text.toString():"",e.appendChild(s),t.hasOwnProperty("disabled")&&t.disabled?e.classList.add("disabled"):e.classList.add("enabled");const n=document.createElement("span");if (n.classList="hotkey",n.innerText=t.hasOwnProperty("hotkey")?t.hotkey.toString():"",e.appendChild(n),t.hasOwnProperty("subitems")&&Array.isArray(t.subitems)&&t.subitems.length>0){const s=new ContextMenu(this.container,t.subitems);s.root=!1,s.parent=this;const n=n=>{if (t.hasOwnProperty("disabled")&&1==t.disabled)return;this.hideSubMenus();const i=this.dom.offsetLeft+this.dom.clientWidth+e.offsetLeft,o=this.dom.offsetTop+e.offsetTop;s.shown?s.hide():s.show(i,o)};this.submenus.push(s),e.classList.add("has-subitems"),e.addEventListener("click",n),e.addEventListener("mousemove",n)}else if (t.hasOwnProperty("submenu")&&t.submenu instanceof ContextMenu){const s=t.submenu;s.root=!1,s.parent=this;const n=n=>{if (t.hasOwnProperty("disabled")&&1==t.disabled)return;this.hideSubMenus();const i=this.dom.offsetLeft+this.dom.clientWidth+e.offsetLeft,o=this.dom.offsetTop+e.offsetTop;s.shown?s.hide():s.show(i,o)};this.submenus.push(s),e.classList.add("has-subitems"),e.addEventListener("click",n),e.addEventListener("mousemove",n)}else e.addEventListener("click",i=>{if (this.hideSubMenus(),!e.classList.contains("disabled"))if (t.hasOwnProperty("onclick")&&"function"==typeof t.onclick){const i={handled:!1,item:e,label:s,hotkey:n,items:this.items,data:t};t.onclick(i),i.handled||this.hide()}else this.hide()}),e.addEventListener("mousemove",t=>{this.hideSubMenus()});return e}hideAll(){!this.root||this.parent?this.parent.hide():this.shown&&(this.hideSubMenus(),this.shown=!1,this.container.removeChild(this.dom),this.parent&&this.parent.shown&&this.parent.hide())}hide(){this.dom&&this.shown&&(this.shown=!1,this.hideSubMenus(),this.container.removeChild(this.dom),this.parent&&this.parent.shown&&this.parent.hide())}hideSubMenus(){for(const t of this.submenus)t.shown&&(t.shown=!1,t.container.removeChild(t.dom)),t.hideSubMenus()}show(t,e){this.dom=this.getMenuDom(),this.dom.style.left=`${t}px`,this.dom.style.top=`${e}px`,this.shown=!0,this.container.appendChild(this.dom)}install(){this.container.addEventListener("contextmenu",this._oncontextmenu),this.container.addEventListener("keydown",this._oncontextmenu_keydown),this.container.addEventListener("click",this._onclick),window.addEventListener("blur",this._onblur)}uninstall(){this.dom=null,this.container.removeEventListener("contextmenu",this._oncontextmenu),this.container.removeEventListener("keydown",this._oncontextmenu_keydown),this.container.removeEventListener("click",this._onclick),window.removeEventListener("blur",this._onblur)}}
function initializeContextMenu()
{
	const cmFileManager = new ContextMenu(document.querySelector('#tbDirectory tbody'),
	[
		{
			text: 'DELETE',
			onclick: e =>
            {
                if (typeof filenameLatestRightClicked === "undefined" || filenameLatestRightClicked === "" || filenameLatestRightClicked === null)
                    return;

                PromptSwal.fire({
                    title: `Type "${filenameLatestRightClicked}" to <span class="colorRed">DELETE</span>`,
                    input: "text",
                    preConfirm: (strText) =>
                    {
                        if ( strText !== filenameLatestRightClicked )
                            Swal.showValidationMessage("Name Not Matched");
                        else
                            executeSimpleRequest("DELETE_FILE", filenameLatestRightClicked);
                    }
                });
			}
		},
		{
			text: 'RENAME',
			onclick: e =>
            {
                if (typeof filenameLatestRightClicked === "undefined" || filenameLatestRightClicked === "" || filenameLatestRightClicked === null)
                    return;

                PromptSwal.fire({
                    title: `"${filenameLatestRightClicked}" to <span class="colorYel">RENAME</span>`,
                    input: "text",
                    preConfirm: (strText) =>
                    {
                        let filename = strText+".TXT";

                        if ( !(/^[A-Z0-9_]{1,8}$/.test(strText)) )
                            Swal.showValidationMessage("[A-Z 0-9 _] Max 8 Letters");
                        else
                        if ( findRow(filename) )
                            Swal.showValidationMessage("Name Already Exists");
                        else
                            executeSimpleRequest("RENAME_FILE", [filenameLatestRightClicked, filename]);
                    }
                });
            }
		}
	]);
	cmFileManager.install();
}



/**⦓ SCRIPT, ABOUT VIEW ⦔**/

const divTerminal = document.querySelector("#divTerminal");
const tbDirectory = document.querySelector("#tbDirectory");

const TerminalWriter =
{
    panelLatestCreated: null,

    addTextInstantly: function(typeThing)
    {
        const panel = document.createElement('p');
        this.panelLatestCreated = panel;
        panel.innerHTML = typeThing;

        divTerminal.appendChild(panel);
        divTerminal.scrollTop = divTerminal.scrollHeight;

        return this;
    },
    addTextWithAnimation: function(typeThing)
    {
        const panel = document.createElement('p');
        this.panelLatestCreated = panel;
        var typewriter = new Typewriter(panel, {
            loop: false,
            delay: 10,
            cursor: '',
        });
        typewriter
            .typeString(typeThing)
            .start();

        divTerminal.appendChild(panel);
        divTerminal.scrollTop = divTerminal.scrollHeight;

        return this;
    },
    attachTooltip: function(tooltipThing)
    {
        if (!this.panelLatestCreated || !this.panelLatestCreated.querySelector('a'))
            return;

        tippy(this.panelLatestCreated.querySelector('a'), {
            placement: 'right',
            allowHTML: true,
            content: tooltipThing.replace(/\n/g, "<br/>")
        });

        return this;
    }
}

const taFileContent = document.querySelector("#taFileContent");
const divFileList = document.querySelector("#divFileList");
const btnCreateFile = document.querySelector("#btnCreateFile");
const btnSaveFile = document.querySelector("#btnSaveFile");
const btnGoToListener = document.querySelector("#btnGoToListener");

let isEnabledTextarea = false;
function enableTextarea(value = taFileContent.value)
{
    taFileContent.value = value;

    taFileContent.readOnly = false;
    taFileContent.style.cursor = "text";
    taFileContent.style.overflowX = "auto";
    taFileContent.style.overflowY = "auto";
    taFileContent.style.color = "#ECECEC";
    taFileContent.style.backgroundColor = "#212121";

    btnCreateFile.style.display = 'none';
    btnSaveFile.style.display = 'block';

    btnGoToListener.style.display = (getCurrentFilename().includes("MAPPER.") || getCurrentFilename().includes("KEYCODE.")) ? 'none' : 'block';

    isEnabledTextarea = true;
}
function disableTextarea()
{
    if (getCurrentFilename().length > 0 && taFileContent.value.length > 0)
        setFiledata(getCurrentFilename(), taFileContent.value);

    taFileContent.readOnly = true;
    taFileContent.style.cursor = "default";
    taFileContent.style.overflowX = "hidden";
    taFileContent.style.overflowY = "hidden";
    taFileContent.style.color = "rgba(0,0,0,0)";
    taFileContent.style.backgroundColor = "#4f4f4f";

    btnCreateFile.style.display = 'block';
    btnSaveFile.style.display = 'none';

    btnGoToListener.style.display = 'none';

    isEnabledTextarea = false;
}
disableTextarea();

function focusSpanFile(filename)
{
    for(var spanFile of divFileList.querySelectorAll('span'))
        spanFile.classList = (spanFile.dataset.filename === filename) ? 'fileSelected' : '';
}
function getSpanFile(filename)
{
    for(var spanFile of divFileList.querySelectorAll('span'))
        if (spanFile.dataset.filename === filename)
            return spanFile;
}
function createSpanFile(filename)
{
    const spanFile = document.createElement('span');
    spanFile.dataset.filename = filename;
    spanFile.innerText = (filename.lastIndexOf('.') > -1) ? filename.substring(0, filename.lastIndexOf('.')) : filename;
    spanFile.addEventListener("click", function()
    {
        disableTextarea();

        if (getCurrentFilename() === this.dataset.filename) {
            enableTextarea();
            return;
        };

        setCurrentFilename(this.dataset.filename);

        enableTextarea(this.dataset.filedata);
        focusSpanFile(this.dataset.filename);
    });

    divFileList.appendChild(spanFile);
    divFileList.style.display = 'block';
}
function deleteSpanFile(filename)
{
    if (getCurrentFilename() === filename) {
        setCurrentFilename('');
        taFileContent.value = '';

        focusSpanFile('');
    }

    for(var spanFile of divFileList.querySelectorAll('span'))
        if (spanFile.dataset.filename === filename)
            spanFile.remove();

    if (divFileList.querySelectorAll('span').length == 0)
        divFileList.style.display = 'none';
}

for(divKey of document.querySelectorAll('.key'))
{
	divKey.setAttribute('draggable', 'false');
	divKey.style.userDrag = 'none';
	divKey.style.userSelect  = 'none';
}

const selectEventList = document.querySelector("#selectEventList");
const selectFunctionList = document.querySelector("#selectFunctionList");

function createOptionToEventList(text, eventDetail, anchorTag = undefined)
{
	const newOption = document.createElement("option");
    newOption.dataset.eventDetail = eventDetail;
    newOption.appendChild( document.createTextNode(text) );

    if (text !== eventDetail)
    {
        const newTip = tippy(newOption, {
            placement: 'left',
            content: eventDetail
        });
        arrTipsForListener.push(newTip);
    }

    if (!anchorTag)
        selectEventList.appendChild(newOption);
    else
        selectEventList.insertBefore(newOption, anchorTag);

	selectEventList.scrollTop = selectEventList.scrollHeight;

    return newOption;
}
function createOptionToFunctionList(text, funcName, funcValue, anchorTag = undefined)
{
	const newOption = document.createElement("option");
    newOption.dataset.funcName = funcName;
    newOption.dataset.funcValue = funcValue;
	newOption.appendChild( document.createTextNode(text) );

    const newTip = tippy(newOption, {
        placement: 'right',
        allowHTML: true,
        content: funcValue.replace(/\n/g, "<br/>")
    });
    arrTipsForListener.push(newTip);

    if (!anchorTag)
        selectFunctionList.appendChild(newOption);
    else
        selectFunctionList.insertBefore(newOption, anchorTag);

	selectFunctionList.scrollTop = selectFunctionList.scrollHeight;

    return newOption;
}

const divEventModifier = document.querySelector("#divEventModifier");
const inpStabilizeDelay = document.querySelector("#inpStabilizeDelay");

const isFixedDelay = () => (inpStabilizeDelay.value.length>0 && !inpStabilizeDelay.classList.contains("colorRed"));
const getFixedDelay = () => inpStabilizeDelay.value;



/**⦓ SCRIPT, ABOUT COMMUNICATION ⦔**/

let resultCombined = '';
let isProcessing = false;
let isErrorOccurred = false;

function SEND_REQUEST(strRequest)
{
    if (isProcessing)
        return;

    isProcessing = true;


    TerminalWriter.addTextInstantly('<br/><br/>');
    TerminalWriter.addTextWithAnimation('SEND REQUEST : '+strRequest);

    resultCombined = '';


    $.ajax({
        url:  '/DarkJunction/SEND_REQUEST',
        type: 'POST',
        data: JSON.stringify({ strRequest: strRequest }),
        contentType: 'application/json; charset=utf-8',
        processData: false, cache: false,
        success: function(data)
        {
            console.log(data);

            if (data.result.length < 1) {
                isErrorOccurred = true;
                TerminalWriter.addTextInstantly('Data Not Arrived');
            }
            else {
                TerminalWriter.addTextInstantly('OK!');
            }

            isProcessing = false;
        },
        error: function()
        {
            alert('SEND_REQUEST ajax Failed');

            isErrorOccurred = true;
            isProcessing = false;
        }
    });
}

function CONTINUE_PULL()
{
    if (isProcessing)
        return;

    isProcessing = true;


    TerminalWriter.addTextInstantly('<br/>');
    TerminalWriter.addTextWithAnimation('now downloading data...');


    $.ajax({
        url:  '/DarkJunction/CONTINUE_PULL',
        type: 'POST',
        // data: nothing,
        contentType: 'application/json; charset=utf-8',
        processData: false, cache: false,
        success: function(data)
        {
            console.log(data);

            TerminalWriter.addTextInstantly('OK!&nbsp;<a>content</a>').attachTooltip(data.result);

            resultCombined += data.result;

            isProcessing = false;
        },
        error: function()
        {
            alert('CONTINUE_PULL ajax Failed');

            isErrorOccurred = true;
            isProcessing = false;
        }
    });
}

function IS_PUSHABLE(arrPushData)
{
    if (isProcessing)
        return false;

    isProcessing = true;


    $.ajax({
        url:  '/DarkJunction/IS_PUSHABLE',
        type: 'POST',
        // data: nothing,
        contentType: 'application/json; charset=utf-8',
        processData: false, cache: false,
        success: function(data)
        {
            console.log(data);

            if (data.result < 0)
                isErrorOccurred = true;
            else
            if (data.result === 1)
                CONTINUE_PUSH(arrPushData.shift());
        },
        error: function()
        {
            alert('IS_PUSHABLE ajax Failed');

            isErrorOccurred = true;
            isProcessing = false;
        }
    });
}
function CONTINUE_PUSH(strPushData)
{
    TerminalWriter.addTextInstantly('<br/>');
    TerminalWriter.addTextWithAnimation('now uploading data...');


    $.ajax({
        url:  '/DarkJunction/CONTINUE_PUSH',
        type: 'POST',
        data: JSON.stringify({ strPushData: strPushData }),
        contentType: 'application/json; charset=utf-8',
        processData: false, cache: false,
        success: function(data)
        {
            TerminalWriter.addTextInstantly('OK!');

            isProcessing = false;
        },
        error: function()
        {
            alert('CONTINUE_PUSH ajax Failed');

            isErrorOccurred = true;
            isProcessing = false;
        }
    });
}

let run;
let isRunning = 0;

function executePullRootData()
{
    if (isRunning++ != 0)
        return;


    $('#tbDirectory').hide();
    $('#divTerminal').show();


    SEND_REQUEST('PULL_ROOT_DATA');

    run = setInterval(() =>
    {
        if (isProcessing)
            return;


        if (resultCombined.includes('7R4N5M15510N_C0MPL373D') || resultCombined.includes('3RR0R3D'))
        {
            clearInterval(run);

            console.log(resultCombined);
            TerminalWriter.addTextInstantly('<br/>REQUEST FINISH');


            // Draw Contents
            {
                let rows = resultCombined.split('\n');
                for(var row of rows) {
                    if (!row.includes('\t'))
                        continue;

                    let cols = row.split('\t');
                    let filename = cols[0];
                    let filesize = cols[1];

                    createRow(filename, filesize);
                }
            }


            $('#divTerminal').hide();
            $('#tbDirectory').show();
            setTimeout(() => { isRunning = 0; }, 500);
        }
        else
        if (isErrorOccurred)
        {
            clearInterval(run);

            DialogSwal.fire({ icon: "error", title: "ERRORED" });

            isErrorOccurred = false;
            setTimeout(() => { isRunning = 0; }, 500);
        }
        else
        {
            CONTINUE_PULL();
        }
    }, 100);
}

function setCurrentFilename(filename)   { taFileContent.dataset.currentFilename = filename; }
function getCurrentFilename()           { return taFileContent.dataset.currentFilename; }

function isExistFiledata(filename)
{
    for(var spanFile of divFileList.querySelectorAll('span'))
        if (spanFile.dataset.filename === filename)
            return true;

    return false;
}
function setFiledata(filename, filedata)
{
    if (filename.length < 1)
        return;

    if (!isExistFiledata(filename))
        createSpanFile(filename);

    getSpanFile(filename).dataset.filedata = filedata;
}
function delFiledata(filename)
{
    if (!isExistFiledata(filename))
        return;

    deleteSpanFile(filename);
}

function executePullFile(filename)
{
    if (isRunning++ != 0)
        return;


    setCurrentFilename(filename)
    taFileContent.value = '';

    $('#tbDirectory').hide();
    $('#divTerminal').show();

    disableTextarea();
    focusSpanFile('');


    SEND_REQUEST(`PULL_FILE("${filename}")`);

    run = setInterval(() =>
    {
        if (isProcessing)
            return;


        if (resultCombined.includes('7R4N5M15510N_C0MPL373D') || resultCombined.includes('3RR0R3D'))
        {
            clearInterval(run);

            console.log(resultCombined);
            TerminalWriter.addTextInstantly('<br/>REQUEST FINISH');

            // DialogSwal.fire({ icon: "success", title: "LOAD COMPLETE" });


            // Update Contents
            {
                resultCombined =    (resultCombined.includes('7R4N5M15510N_C0MPL373D'))
                                    ? resultCombined.substring(0, resultCombined.lastIndexOf('7R4N5M15510N'))
                                    : '3RR0R3D';

                if (resultCombined !== '3RR0R3D') {
                    setFiledata(getCurrentFilename(), resultCombined);

                    enableTextarea(resultCombined);
                    focusSpanFile(getCurrentFilename());
                }
                else {
                    delFiledata(getCurrentFilename());
                    setCurrentFilename('');
                }
            }


            $('#divTerminal').hide();
            $('#tbDirectory').show();
            setTimeout(() => { isRunning = 0; }, 500);
        }
        else
        if (isErrorOccurred)
        {
            clearInterval(run);

            DialogSwal.fire({ icon: "error", title: "ERRORED" });

            isErrorOccurred = false;
            setTimeout(() => { isRunning = 0; }, 500);
        }
        else
        {
            CONTINUE_PULL();
        }
    }, 100);
}

function splitTextByBytes(text, bytes)
{
    let result = [];

    let currentBytes = 0;
    let startIndex = 0;

    for(let i = 0; i < text.length; i++) {
        const charCode = text.charCodeAt(i);
        const byteOfCharCode = (charCode <= 127) ? 1 : 3; // ASCII=1Byte, Others=3Byte

        if (currentBytes + byteOfCharCode <= bytes)
            currentBytes += byteOfCharCode;
        else {
            result.push(text.substring(startIndex, i));
            startIndex = i;
            currentBytes = byteOfCharCode;
        }
    }
    if (startIndex < text.length)
        result.push(text.substring(startIndex));

    return result;
}

let dataSplited = [];
function executePushFile(filename)
{
    if (!isEnabledTextarea || getCurrentFilename().length < 1)
        return;

    if (isRunning++ != 0)
        return;


    if (taFileContent.value.slice(-1) != '\n')
        taFileContent.value += '\n';
    dataSplited = splitTextByBytes(taFileContent.value, 512);
    dataSplited.push('7R4N5M15510N_C0MPL373D');

    $('#tbDirectory').hide();
    $('#divTerminal').show();
    disableTextarea();


    SEND_REQUEST(`PUSH_FILE("${filename}")`);

    run = setInterval(() =>
    {
        if (isProcessing)
            return;


        if (dataSplited.length == 0)
        {
            if (isProcessing)
                return;


            clearInterval(run);

            TerminalWriter.addTextInstantly('<br/>REQUEST FINISH');

            let isJustCreateFile = !findRow(getCurrentFilename());
            if (isJustCreateFile)
                DialogSwal.fire({ icon: "success", title: "CREATE COMPLETE" });
            else
                DialogSwal.fire({ icon: "success", title: "SAVE COMPLETE" });


            // Update Contents
            {
                setFiledata(getCurrentFilename(), taFileContent.value);

                // if file just created by btnCreateFile
                if (isJustCreateFile)
                {
                    createRow(getCurrentFilename(), 0);

                    enableTextarea('');
                    focusSpanFile(getCurrentFilename());
                }
            }


            $('#divTerminal').hide();
            $('#tbDirectory').show();
            setTimeout(() => { isRunning = 0; }, 500);
        }
        else
        if (isErrorOccurred)
        {
            clearInterval(run);

            DialogSwal.fire({ icon: "error", title: "ERRORED" });

            while(dataSplited.length > 0)
                dataSplited.pop();

            isErrorOccurred = false;
            setTimeout(() => { isRunning = 0; }, 500);
        }
        else
        {
            IS_PUSHABLE(dataSplited);
        }
    }, 500);
}

function executeSimpleRequest(strRequest, strParam = null)
{
    if (isRunning++ != 0)
        return;


    $('#tbDirectory').hide();
    $('#divTerminal').show();
    disableTextarea();


    if (strParam == null)
        SEND_REQUEST(strRequest);
    else
    if (!Array.isArray(strParam))
        SEND_REQUEST(`${strRequest}("${strParam}")`);
    else {
        let strParamFormatted = strParam.map(param => `"${param}"`).join('|');
        SEND_REQUEST(`${strRequest}(${strParamFormatted})`);
    }

    run = setInterval(() =>
    {
        if (isProcessing)
            return;


        if (isErrorOccurred)
        {
            clearInterval(run);

            DialogSwal.fire({ icon: "error", title: "ERRORED" });

            isErrorOccurred = false;
            setTimeout(() => { isRunning = 0; }, 500);
        }
        else
        {
            clearInterval(run);

            console.log(resultCombined);
            TerminalWriter.addTextInstantly('<br/>REQUEST FINISH');


            // Draw Contents
            {
                if (strRequest.includes('DELETE_FILE'))
                {
                    let filename = strParam;

                    deleteRow(filename);
                    delFiledata(filename);
                }
                if (strRequest.includes('RENAME_FILE'))
                {
                    let filenameOld = strParam[0];
                    let filenameNew = strParam[1];

                    createRow(filenameNew, getRowData(filenameOld)[1]);
                    deleteRow(filenameOld);
                    delFiledata(filenameOld);
                }
            }


            $('#divTerminal').hide();
            $('#tbDirectory').show();
            setTimeout(() => { isRunning = 0; }, 500);
        }
    }, 100);
}



/**⦓ SCRIPT, ABOUT RUN ⦔**/

let currentMouseX = 0;
let currentMouseY = 0;
document.addEventListener('mousemove', (event) => { currentMouseX = event.clientX; currentMouseY = event.clientY; });

function isCursorInsideNode(node)
{   const rect = node.getBoundingClientRect(); return (currentMouseX >= rect.left && currentMouseX <= rect.right && currentMouseY >= rect.top && currentMouseY <= rect.bottom);   }

let timeLatestKeyEventCame = -1;
let arrTipsForListener = [];
function resetListener()
{
    selectEventList.innerHTML = '';
    selectFunctionList.innerHTML = '';

    for(tip of arrTipsForListener)
        tip.destroy();
    arrTipsForListener = [];

    for(const e of document.querySelector('.keyboard').querySelectorAll('.pressed'))
        e.classList.remove('pressed');

    timeLatestKeyEventCame = -1;
}

$( document ).ready(function()
{
    // Initialize View
    {
        initializePagePiling();
        initializeSplit();
        document.querySelector('#pagepiling').style.display = 'block';
    }


    // Load Contents & Enhance visibility
    {
        TerminalWriter.addTextWithAnimation('Please Wait . . .');
        executePullRootData();

        initializeDataTable();
        initializeContextMenu();
    }


    // Define global keyboard & mouse event Listeners
    document.addEventListener('keydown', function(event)
    {
        if (isRunning != 0)
            return;

        if (nowSection == SECTION_EDITOR)
        {
            if (event.key === 'Escape') {
                if (isEnabledTextarea)
                    disableTextarea();
                else {
                    setCurrentFilename('');
                    taFileContent.value = '';

                    focusSpanFile('');
                }
            }
            else // Ctrl+S
            if (event.ctrlKey && event.key === 's') {
                if (!isEnabledTextarea)
                    return;

                event.preventDefault();
                executePushFile(getCurrentFilename());
            }
        }
        else
        if (nowSection == SECTION_LISTENER)
        {
            if (isCursorInsideNode(divEventModifier.parentNode))
                return;

            if (!event.code)
                return;

            if ($(eventCode_To_viewId(event.code)).hasClass('pressed'))
                return;
            else
                $(eventCode_To_viewId(event.code)).addClass("pressed");


            event.preventDefault();

            if (timeLatestKeyEventCame != -1) {
                if (isFixedDelay())
                    createOptionToEventList(duplicator = getFixedDelay() + "ms", duplicator);
                else
                    createOptionToEventList(duplicator = (Date.now()-timeLatestKeyEventCame) + "ms", duplicator);
            }
            createOptionToEventList(duplicator = "DN " + eventCode_To_keyStr(event.code), duplicator);

            timeLatestKeyEventCame = Date.now();
        }
    });
    document.addEventListener('keyup', function(event)
    {
        if (isRunning != 0)
            return;

        if (nowSection == SECTION_EDITOR)
        {
            ;
        }
        else
        if (nowSection == SECTION_LISTENER)
        {
            if (isCursorInsideNode(divEventModifier.parentNode))
                return;

            if (!event.code)
                return;

            if (!$(eventCode_To_viewId(event.code)).hasClass('pressed'))
                return;
            else
                $(eventCode_To_viewId(event.code)).removeClass("pressed");


            event.preventDefault();

            if (isFixedDelay())
                createOptionToEventList(duplicator = getFixedDelay() + "ms", duplicator);
            else
                createOptionToEventList(duplicator = (Date.now()-timeLatestKeyEventCame) + "ms", duplicator);
            createOptionToEventList(duplicator = "UP " + eventCode_To_keyStr(event.code), duplicator);

            timeLatestKeyEventCame = Date.now();
        }
    });
    document.addEventListener("mousedown", function(event)
    {
        if (isRunning != 0)
            return;

        if (nowSection == SECTION_EDITOR)
        {
            ;
        }
        else
        if (nowSection == SECTION_LISTENER)
        {
            if (!isCursorInsideNode(divEventModifier.parentNode)) {
                for(op of selectEventList.querySelectorAll('option')) 
                    op.selected = false;
                for(op of selectFunctionList.querySelectorAll('option'))
                    op.selected = false;
            }
        }
    });


    // divPanelL can be swap its content by wheel up/down
    document.querySelector("#divPanelL").addEventListener('wheel', function(event)
    {
        if (isRunning != 0)
            return;

		const divPanelL = document.querySelector("#divPanelL");
        const currentChildIndex = (() =>
        {
            for(var i = 0; i < divPanelL.children.length; i++) {
                if (divPanelL.children[i].style.display !== 'none')
                    return i;
            }

            return 0;
        })();
        let currentChildNode  = divPanelL.children[currentChildIndex];

        // when wheel up evnets
        if (event.deltaY < 0 && currentChildIndex != 0)
        {
            // before swapping
            {
                // Only through thead, tbDirectory can be swapped
                if ( currentChildNode.contains(tbDirectory) ) {
                    if (!isCursorInsideNode(tbDirectory.querySelector('thead')))
                        return;
                }
            }

            $(currentChildNode).hide();
            $(currentChildNode = divPanelL.children[currentChildIndex-1]).show();
            currentChildNode.scrollTop = 0;

            // after swapping
            {
                // divTerminal always starts from bottom of the scroll
                if ( currentChildNode.contains(divTerminal) )
                    divTerminal.scrollTop = divTerminal.scrollHeight;
            }
        }
        else
        // when wheel down evnets
        if (event.deltaY > 0 && currentChildIndex != divPanelL.children.length-1)
        {
            // before swapping
            {
            }

            $(currentChildNode).hide();
            $(currentChildNode = divPanelL.children[currentChildIndex+1]).show();
            currentChildNode.scrollTop = 0;

            // after swapping
            {
            }
        }
    });


    // Allows writing '\t' in the textarea
    taFileContent.addEventListener('keydown', function(event)
    {
        if (isRunning != 0)
            return;

        if (!isEnabledTextarea)
            return;

        if (event.key === 'Tab') {
            event.preventDefault();

            const start = taFileContent.selectionStart;
            const end = taFileContent.selectionEnd;

            taFileContent.value = taFileContent.value.substring(0, start) + '\t' + taFileContent.value.substring(end);

            taFileContent.selectionStart = taFileContent.selectionEnd = start + 1;
        }
    });


    // can exit textarea's temporary disable state
    taFileContent.addEventListener("click", function(event)
    {
        if (isRunning != 0)
            return;

        if (!isEnabledTextarea && getCurrentFilename().length > 1)
            enableTextarea();
    });


    // Buttons that located at SECTION_EDITOR's bottom right
    btnCreateFile.addEventListener("click", function(event)
    {
        PromptSwal.fire({
            title: `Type Filename for <span class="colorGre">CREATE</span>`,
            input: "text",
            preConfirm: (strText) =>
            {
                let filename = strText+".TXT";

                if ( !(/^[A-Z0-9_]{1,8}$/.test(strText)) )
                    Swal.showValidationMessage("[A-Z 0-9 _] Max 8 Letters");
                else
                if ( findRow(filename) )
                    Swal.showValidationMessage("Name Already Exists");
                else {
                    setCurrentFilename(filename);
                    taFileContent.value = '';
                    enableTextarea();

                    executePushFile(filename);
                }
            }
        });
    });
    btnSaveFile.addEventListener("click", function(event)
    {
        executePushFile(getCurrentFilename());
    });


    // Virtual Keyboard that located at SECTION_LISTENER's top
    document.querySelector('.keyboard').addEventListener('click', function(event)
    {
        event.preventDefault();

        let elementClickedKey = event.target.closest('.key');
        if (!elementClickedKey)
            return;

        if ($( '#'+elementClickedKey.id ).hasClass('pressed'))
            return;


        $( '#'+elementClickedKey.id ).addClass("pressed");
        setTimeout(() => { $( '#'+elementClickedKey.id ).removeClass("pressed"); }, 30);

        let text;
        if (timeLatestKeyEventCame != -1)
            createOptionToEventList(duplicator = (Date.now()-timeLatestKeyEventCame) + "ms", duplicator);
        createOptionToEventList(duplicator = "DNUP " + viewId_To_keyStr(elementClickedKey.id), duplicator);

        timeLatestKeyEventCame = Date.now();
    });
    document.querySelector('.keyboard').addEventListener("contextmenu", function(event)
    {
        event.preventDefault();

        let elementClickedKey = event.target.closest('.key');
        if (!elementClickedKey)
            return;


        if ( !$( '#'+elementClickedKey.id ).hasClass('pressed') )
        {
            $( '#'+elementClickedKey.id ).addClass("pressed");

            if (timeLatestKeyEventCame != -1)
                createOptionToEventList(duplicator = (Date.now()-timeLatestKeyEventCame) + "ms", duplicator);
            createOptionToEventList(duplicator = "DN " + viewId_To_keyStr(elementClickedKey.id), duplicator);
        }
        else
        {
            $( '#'+elementClickedKey.id ).removeClass("pressed");

            createOptionToEventList(duplicator = (Date.now()-timeLatestKeyEventCame) + "ms", duplicator);
            createOptionToEventList(duplicator = "UP " + viewId_To_keyStr(elementClickedKey.id), duplicator);
        }

        timeLatestKeyEventCame = Date.now();
    });


    // Inputs that located at SECTION_LISTENER's bottom
    document.querySelector('#selectEventList').addEventListener("dblclick", function(event)
    {
        let isExistSelected = [...selectEventList.querySelectorAll('option')].find(option => option.selected);
        if (!isExistSelected)
            return;

        if (isCursorInsideNode(isExistSelected)
            && isExistSelected.textContent.includes("ms"))
        {
            let msDelay = prompt("Delay(ms) : ");
            if (!(/^\d+$/.test(msDelay)) && !(/^\d+\s*~\s*\d+$/.test(msDelay)))
                return;

            if (isExistSelected.dataset.eventDetail.includes("//")) {
                isExistSelected.textContent = msDelay + "ms " + isExistSelected.textContent.slice(isExistSelected.textContent.indexOf("//"));
                isExistSelected.dataset.eventDetail = msDelay + "ms " + isExistSelected.dataset.eventDetail.slice(isExistSelected.dataset.eventDetail.indexOf("//"));
                isExistSelected._tippy.setContent( isExistSelected.dataset.eventDetail );
            }
            else {
                isExistSelected.textContent = msDelay + "ms";
                isExistSelected.dataset.eventDetail = msDelay + "ms";
            }
        }
    });

    inpStabilizeDelay.addEventListener("change", function(event)
    {
        if (!(/^\d+$/.test(inpStabilizeDelay.value)) && !(/^\d+\s*~\s*\d+$/.test(inpStabilizeDelay.value)))
            inpStabilizeDelay.classList.add("colorRed");
        else
            inpStabilizeDelay.classList.remove("colorRed");
    });
    inpStabilizeDelay.addEventListener("click", function(event)
    {
        if (inpStabilizeDelay.value.length != 0)
            return;

        inpStabilizeDelay.value = "23~51";
        inpStabilizeDelay.classList.remove("colorRed");
    });
    document.querySelector("#btnAddDelay").addEventListener("click", function(event)
    {
        let msDelay = prompt("Delay(ms) : ");
        if (!(/^\d+$/.test(msDelay)) && !(/^\d+\s*~\s*\d+$/.test(msDelay)))
            return;


        let text;
        let anchorTag = [...selectEventList.querySelectorAll('option')].find(option => option.selected);
        if (anchorTag)
            createOptionToEventList(duplicator = msDelay+'ms', duplicator, anchorTag);
        else
            createOptionToEventList(duplicator = msDelay+'ms', duplicator);

        timeLatestKeyEventCame = -1;
    });

    document.querySelector("#btnMakeFunction").addEventListener("click", function(event)
    {
        let isExistSelected = [...selectEventList.querySelectorAll('option')].find(option => option.selected);
        if (!isExistSelected)
            return;

        let funcName = prompt("Function Name [A-Za-z0-9_]{1,8} : ");
        if (!(/^[A-Za-z0-9_]{1,8}$/.test(funcName)))
            return;


        // update #selectFunctionList
        {
            let funcValue = "{\n";
            for(op of selectEventList.querySelectorAll('option'))
                if (op.selected)
                    funcValue += '\t' + op.textContent + '\n';
            funcValue = funcValue + "}";

            createOptionToFunctionList("@"+funcName, funcName, funcValue);
        }

        // update #selectEventList
        {
            let anchorTag = [...selectEventList.querySelectorAll('option')].find(option => option.selected);

            createOptionToEventList("$"+funcName, "$"+funcName, anchorTag);

            for(op of selectEventList.querySelectorAll('option'))
                if (op.selected)
                    op.remove();
        }

        timeLatestKeyEventCame = -1;
    });
    document.querySelector("#btnCallFunction").addEventListener("click", function(event)
    {
        let isExistSelected = [...selectFunctionList.querySelectorAll('option')].find(option => option.selected);
        if (!isExistSelected)
            return;


        let funcName = isExistSelected.dataset.funcName;
        let anchorTag = [...selectEventList.querySelectorAll('option')].find(option => option.selected);
        if (anchorTag)
            createOptionToEventList("$"+funcName, "$"+funcName, anchorTag);
        else
            createOptionToEventList("$"+funcName, "$"+funcName);

        timeLatestKeyEventCame = -1;
    });
    document.querySelector("#btnDeleteEvent").addEventListener("click", function(event)
    {
        let isExistSelected;

        isExistSelected = false;
        for(op of selectEventList.querySelectorAll('option'))
            if (isExistSelected = op.selected)
                break;

        if (isExistSelected)
        {
            for(op of selectEventList.querySelectorAll('option')){
                if (op.selected)
                    op.remove();
            }
        }

        isExistSelected = false;
        for(op of selectFunctionList.querySelectorAll('option'))
            if (isExistSelected = op.selected)
                break;

        if (isExistSelected)
        {
            for(op of selectFunctionList.querySelectorAll('option')){
                if (op.selected)
                {
                    for(opop of selectEventList.querySelectorAll('option')){
                        if (opop.dataset.funcName === op.dataset.funcName)
                            opop.remove();
                    }

                    op.remove();
                }
            }
        }
    });
    document.querySelector("#btnDeleteEventAll").addEventListener("click", function(event)
    {
        if (!confirm("DELETE ALL REALLY?"))
            return;

        for(op of selectEventList.querySelectorAll('option'))
            op.remove();
        for(op of selectFunctionList.querySelectorAll('option'))
            op.remove();
    });
});
function eventCode_To_keyStr(a){switch(a){case"KeyA":return"KEY_A";case"KeyB":return"KEY_B";case"KeyC":return"KEY_C";case"KeyD":return"KEY_D";case"KeyE":return"KEY_E";case"KeyF":return"KEY_F";case"KeyG":return"KEY_G";case"KeyH":return"KEY_H";case"KeyI":return"KEY_I";case"KeyJ":return"KEY_J";case"KeyK":return"KEY_K";case"KeyL":return"KEY_L";case"KeyM":return"KEY_M";case"KeyN":return"KEY_N";case"KeyO":return"KEY_O";case"KeyP":return"KEY_P";case"KeyQ":return"KEY_Q";case"KeyR":return"KEY_R";case"KeyS":return"KEY_S";case"KeyT":return"KEY_T";case"KeyU":return"KEY_U";case"KeyV":return"KEY_V";case"KeyW":return"KEY_W";case"KeyX":return"KEY_X";case"KeyY":return"KEY_Y";case"KeyZ":return"KEY_Z";case"Digit1":return"KEY_1";case"Digit2":return"KEY_2";case"Digit3":return"KEY_3";case"Digit4":return"KEY_4";case"Digit5":return"KEY_5";case"Digit6":return"KEY_6";case"Digit7":return"KEY_7";case"Digit8":return"KEY_8";case"Digit9":return"KEY_9";case"Digit0":return"KEY_0";case"Enter":return"KEY_ENTER";case"Escape":return"KEY_ESC";case"Backspace":return"KEY_BACKSPACE";case"Tab":return"KEY_TAB";case"Space":return"KEY_SPACE";case"Minus":return"KEY_MINUS";case"Equal":return"KEY_EQUAL";case"BracketLeft":return"KEY_LEFT_BRACE";case"BracketRight":return"KEY_RIGHT_BRACE";case"Backslash":return"KEY_BACKSLASH";case"Semicolon":return"KEY_SEMICOLON";case"Quote":return"KEY_QUOTE";case"Backquote":return"KEY_TILDE";case"Comma":return"KEY_COMMA";case"Period":return"KEY_PERIOD";case"Slash":return"KEY_SLASH";case"CapsLock":return"KEY_CAPS_LOCK";case"F1":return"KEY_F1";case"F2":return"KEY_F2";case"F3":return"KEY_F3";case"F4":return"KEY_F4";case"F5":return"KEY_F5";case"F6":return"KEY_F6";case"F7":return"KEY_F7";case"F8":return"KEY_F8";case"F9":return"KEY_F9";case"F10":return"KEY_F10";case"F11":return"KEY_F11";case"F12":return"KEY_F12";case"PrintScreen":return"KEY_PRINTSCREEN";case"ScrollLock":return"KEY_SCROLL_LOCK";case"Pause":return"KEY_PAUSE";case"Insert":return"KEY_INSERT";case"Home":return"KEY_HOME";case"PageUp":return"KEY_PAGE_UP";case"Delete":return"KEY_DELETE";case"End":return"KEY_END";case"PageDown":return"KEY_PAGE_DOWN";case"ArrowRight":return"KEY_RIGHT";case"ArrowLeft":return"KEY_LEFT";case"ArrowDown":return"KEY_DOWN";case"ArrowUp":return"KEY_UP";case"NumLock":return"KEY_NUM_LOCK";case"NumpadDivide":return"KEYPAD_DIVIDE";case"NumpadMultiply":return"KEYPAD_MULTIPLY";case"NumpadSubtract":return"KEYPAD_SUBTRACT";case"NumpadAdd":return"KEYPAD_ADD";case"NumpadEnter":return"KEYPAD_ENTER";case"Numpad1":return"KEYPAD_1";case"Numpad2":return"KEYPAD_2";case"Numpad3":return"KEYPAD_3";case"Numpad4":return"KEYPAD_4";case"Numpad5":return"KEYPAD_5";case"Numpad6":return"KEYPAD_6";case"Numpad7":return"KEYPAD_7";case"Numpad8":return"KEYPAD_8";case"Numpad9":return"KEYPAD_9";case"Numpad0":return"KEYPAD_0";case"NumpadDecimal":return"KEYPAD_PERIOD";case"ControlLeft":return"KEY_LEFT_CTRL";case"ShiftLeft":return"KEY_LEFT_SHIFT";case"AltLeft":return"KEY_LEFT_ALT";case"MetaLeft":return"KEY_LEFT_GUI";case"ControlRight":return"KEY_RIGHT_CTRL";case"ShiftRight":return"KEY_RIGHT_SHIFT";case"AltRight":return"KEY_RIGHT_ALT";case"ContextMenu":return"KEY_CONTEXT"}};
function eventCode_To_viewId(a){switch(a){case"KeyA":return"#key_a";case"KeyB":return"#key_b";case"KeyC":return"#key_c";case"KeyD":return"#key_d";case"KeyE":return"#key_e";case"KeyF":return"#key_f";case"KeyG":return"#key_g";case"KeyH":return"#key_h";case"KeyI":return"#key_i";case"KeyJ":return"#key_j";case"KeyK":return"#key_k";case"KeyL":return"#key_l";case"KeyM":return"#key_m";case"KeyN":return"#key_n";case"KeyO":return"#key_o";case"KeyP":return"#key_p";case"KeyQ":return"#key_q";case"KeyR":return"#key_r";case"KeyS":return"#key_s";case"KeyT":return"#key_t";case"KeyU":return"#key_u";case"KeyV":return"#key_v";case"KeyW":return"#key_w";case"KeyX":return"#key_x";case"KeyY":return"#key_y";case"KeyZ":return"#key_z";case"Digit1":return"#key_one";case"Digit2":return"#key_two";case"Digit3":return"#key_three";case"Digit4":return"#key_four";case"Digit5":return"#key_five";case"Digit6":return"#key_six";case"Digit7":return"#key_seven";case"Digit8":return"#key_eight";case"Digit9":return"#key_nine";case"Digit0":return"#key_zero";case"Enter":return"#key_enter";case"Escape":return"#key_esc";case"Backspace":return"#key_backspace";case"Tab":return"#key_tab";case"Space":return"#key_space";case"Minus":return"#key_hyphen";case"Equal":return"#key_equals";case"BracketLeft":return"#key_left_bracket";case"BracketRight":return"#key_right_bracket";case"Backslash":return"#key_backslash";case"Semicolon":return"#key_semicolon";case"Quote":return"#key_apostrophe";case"Backquote":return"#key_accent";case"Comma":return"#key_comma";case"Period":return"#key_period";case"Slash":return"#key_forwardslash";case"CapsLock":return"#key_caps_lock";case"F1":return"#key_f1";case"F2":return"#key_f2";case"F3":return"#key_f3";case"F4":return"#key_f4";case"F5":return"#key_f5";case"F6":return"#key_f6";case"F7":return"#key_f7";case"F8":return"#key_f8";case"F9":return"#key_f9";case"F10":return"#key_f10";case"F11":return"#key_f11";case"F12":return"#key_f12";case"PrintScreen":return"#key_print";case"ScrollLock":return"#key_scroll_lock";case"Pause":return"#key_pause_break";case"Insert":return"#key_insert";case"Home":return"#key_home";case"PageUp":return"#key_page_up";case"Delete":return"#key_delete";case"End":return"#key_end";case"PageDown":return"#key_page_down";case"ArrowRight":return"#key_right";case"ArrowLeft":return"#key_left";case"ArrowDown":return"#key_down";case"ArrowUp":return"#key_up";case"NumLock":return"#key_num_lock";case"NumpadDivide":return"#key_divide";case"NumpadMultiply":return"#key_multiply";case"NumpadSubtract":return"#key_subtract";case"NumpadAdd":return"#key_add";case"NumpadEnter":return"#key_num_enter";case"Numpad1":return"#key_num_1";case"Numpad2":return"#key_num_2";case"Numpad3":return"#key_num_3";case"Numpad4":return"#key_num_4";case"Numpad5":return"#key_num_5";case"Numpad6":return"#key_num_6";case"Numpad7":return"#key_num_7";case"Numpad8":return"#key_num_8";case"Numpad9":return"#key_num_9";case"Numpad0":return"#key_num_0";case"NumpadDecimal":return"#key_num_decimal";case"ControlLeft":return"#key_left_ctrl";case"ShiftLeft":return"#key_left_shift";case"AltLeft":return"#key_left_alt";case"MetaLeft":return"#key_left_gui";case"ControlRight":return"#key_right_ctrl";case"ShiftRight":return"#key_right_shift";case"AltRight":return"#key_right_alt";case"ContextMenu":return"#key_right_app"}};
function viewId_To_keyStr(a){switch(a){case"key_a":return"KEY_A";case"key_b":return"KEY_B";case"key_c":return"KEY_C";case"key_d":return"KEY_D";case"key_e":return"KEY_E";case"key_f":return"KEY_F";case"key_g":return"KEY_G";case"key_h":return"KEY_H";case"key_i":return"KEY_I";case"key_j":return"KEY_J";case"key_k":return"KEY_K";case"key_l":return"KEY_L";case"key_m":return"KEY_M";case"key_n":return"KEY_N";case"key_o":return"KEY_O";case"key_p":return"KEY_P";case"key_q":return"KEY_Q";case"key_r":return"KEY_R";case"key_s":return"KEY_S";case"key_t":return"KEY_T";case"key_u":return"KEY_U";case"key_v":return"KEY_V";case"key_w":return"KEY_W";case"key_x":return"KEY_X";case"key_y":return"KEY_Y";case"key_z":return"KEY_Z";case"key_one":return"KEY_1";case"key_two":return"KEY_2";case"key_three":return"KEY_3";case"key_four":return"KEY_4";case"key_five":return"KEY_5";case"key_six":return"KEY_6";case"key_seven":return"KEY_7";case"key_eight":return"KEY_8";case"key_nine":return"KEY_9";case"key_zero":return"KEY_0";case"key_enter":return"KEY_ENTER";case"key_esc":return"KEY_ESC";case"key_backspace":return"KEY_BACKSPACE";case"key_tab":return"KEY_TAB";case"key_space":return"KEY_SPACE";case"key_hyphen":return"KEY_MINUS";case"key_equals":return"KEY_EQUAL";case"key_left_bracket":return"KEY_LEFT_BRACE";case"key_right_bracket":return"KEY_RIGHT_BRACE";case"key_backslash":return"KEY_BACKSLASH";case"key_semicolon":return"KEY_SEMICOLON";case"key_apostrophe":return"KEY_QUOTE";case"key_accent":return"KEY_TILDE";case"key_comma":return"KEY_COMMA";case"key_period":return"KEY_PERIOD";case"key_forwardslash":return"KEY_SLASH";case"key_caps_lock":return"KEY_CAPS_LOCK";case"key_f1":return"KEY_F1";case"key_f2":return"KEY_F2";case"key_f3":return"KEY_F3";case"key_f4":return"KEY_F4";case"key_f5":return"KEY_F5";case"key_f6":return"KEY_F6";case"key_f7":return"KEY_F7";case"key_f8":return"KEY_F8";case"key_f9":return"KEY_F9";case"key_f10":return"KEY_F10";case"key_f11":return"KEY_F11";case"key_f12":return"KEY_F12";case"key_print":return"KEY_PRINTSCREEN";case"key_scroll_lock":return"KEY_SCROLL_LOCK";case"key_pause_break":return"KEY_PAUSE";case"key_insert":return"KEY_INSERT";case"key_home":return"KEY_HOME";case"key_page_up":return"KEY_PAGE_UP";case"key_delete":return"KEY_DELETE";case"key_end":return"KEY_END";case"key_page_down":return"KEY_PAGE_DOWN";case"key_right":return"KEY_RIGHT";case"key_left":return"KEY_LEFT";case"key_down":return"KEY_DOWN";case"key_up":return"KEY_UP";case"key_num_lock":return"KEY_NUM_LOCK";case"key_divide":return"KEYPAD_DIVIDE";case"key_multiply":return"KEYPAD_MULTIPLY";case"key_subtract":return"KEYPAD_SUBTRACT";case"key_add":return"KEYPAD_ADD";case"key_num_enter":return"KEYPAD_ENTER";case"key_num_1":return"KEYPAD_1";case"key_num_2":return"KEYPAD_2";case"key_num_3":return"KEYPAD_3";case"key_num_4":return"KEYPAD_4";case"key_num_5":return"KEYPAD_5";case"key_num_6":return"KEYPAD_6";case"key_num_7":return"KEYPAD_7";case"key_num_8":return"KEYPAD_8";case"key_num_9":return"KEYPAD_9";case"key_num_0":return"KEYPAD_0";case"key_num_decimal":return"KEYPAD_PERIOD";case"key_left_ctrl":return"KEY_LEFT_CTRL";case"key_left_shift":return"KEY_LEFT_SHIFT";case"key_left_alt":return"KEY_LEFT_ALT";case"key_left_gui":return"KEY_LEFT_GUI";case"key_right_ctrl":return"KEY_RIGHT_CTRL";case"key_right_shift":return"KEY_RIGHT_SHIFT";case"key_right_alt":return"KEY_RIGHT_ALT";case"key_right_app":return"KEY_CONTEXT"}};
function keycode_To_keyStr(a){switch(a.slice(2)){case"00":return"KEY_RESERVED";case"01":return"KEY_ERROR_ROLLOVER";case"02":return"KEY_POST_FAIL";case"03":return"KEY_ERROR_UNDEFINED";case"04":return"KEY_A";case"05":return"KEY_B";case"06":return"KEY_C";case"07":return"KEY_D";case"08":return"KEY_E";case"09":return"KEY_F";case"0A":return"KEY_G";case"0B":return"KEY_H";case"0C":return"KEY_I";case"0D":return"KEY_J";case"0E":return"KEY_K";case"0F":return"KEY_L";case"10":return"KEY_M";case"11":return"KEY_N";case"12":return"KEY_O";case"13":return"KEY_P";case"14":return"KEY_Q";case"15":return"KEY_R";case"16":return"KEY_S";case"17":return"KEY_T";case"18":return"KEY_U";case"19":return"KEY_V";case"1A":return"KEY_W";case"1B":return"KEY_X";case"1C":return"KEY_Y";case"1D":return"KEY_Z";case"1E":return"KEY_1";case"1F":return"KEY_2";case"20":return"KEY_3";case"21":return"KEY_4";case"22":return"KEY_5";case"23":return"KEY_6";case"24":return"KEY_7";case"25":return"KEY_8";case"26":return"KEY_9";case"27":return"KEY_0";case"28":return"KEY_ENTER";case"29":return"KEY_ESC";case"2A":return"KEY_BACKSPACE";case"2B":return"KEY_TAB";case"2C":return"KEY_SPACE";case"2D":return"KEY_MINUS";case"2E":return"KEY_EQUAL";case"2F":return"KEY_LEFT_BRACE";case"30":return"KEY_RIGHT_BRACE";case"31":return"KEY_BACKSLASH";case"32":return"KEY_NON_US_NUM";case"33":return"KEY_SEMICOLON";case"34":return"KEY_QUOTE";case"35":return"KEY_TILDE";case"36":return"KEY_COMMA";case"37":return"KEY_PERIOD";case"38":return"KEY_SLASH";case"39":return"KEY_CAPS_LOCK";case"3A":return"KEY_F1";case"3B":return"KEY_F2";case"3C":return"KEY_F3";case"3D":return"KEY_F4";case"3E":return"KEY_F5";case"3F":return"KEY_F6";case"40":return"KEY_F7";case"41":return"KEY_F8";case"42":return"KEY_F9";case"43":return"KEY_F10";case"44":return"KEY_F11";case"45":return"KEY_F12";case"46":return"KEY_PRINTSCREEN";case"47":return"KEY_SCROLL_LOCK";case"48":return"KEY_PAUSE";case"49":return"KEY_INSERT";case"4A":return"KEY_HOME";case"4B":return"KEY_PAGE_UP";case"4C":return"KEY_DELETE";case"4D":return"KEY_END";case"4E":return"KEY_PAGE_DOWN";case"4F":return"KEY_RIGHT";case"50":return"KEY_LEFT";case"51":return"KEY_DOWN";case"52":return"KEY_UP";case"53":return"KEY_NUM_LOCK";case"54":return"KEYPAD_DIVIDE";case"55":return"KEYPAD_MULTIPLY";case"56":return"KEYPAD_SUBTRACT";case"57":return"KEYPAD_ADD";case"58":return"KEYPAD_ENTER";case"59":return"KEYPAD_1";case"5A":return"KEYPAD_2";case"5B":return"KEYPAD_3";case"5C":return"KEYPAD_4";case"5D":return"KEYPAD_5";case"5E":return"KEYPAD_6";case"5F":return"KEYPAD_7";case"60":return"KEYPAD_8";case"61":return"KEYPAD_9";case"62":return"KEYPAD_0";case"63":return"KEYPAD_DOT";case"64":return"KEY_NON_US";case"65":return"KEY_APPLICATION";case"90":return"KEY_KORENG";case"67":return"KEY_LEFT_CTRL";case"68":return"KEY_LEFT_SHIFT";case"69":return"KEY_LEFT_ALT";case"6A":return"KEY_LEFT_GUI";case"6B":return"KEY_RIGHT_CTRL";case"6C":return"KEY_RIGHT_SHIFT";case"6D":return"KEY_RIGHT_ALT";case"6E":return"KEY_RIGHT_GUI";case"81":return"KEY_SYSTEM_POWER_DOWN";case"82":return"KEY_SYSTEM_SLEEP";case"83":return"KEY_SYSTEM_WAKE_UP";case"B0":return"KEY_MEDIA_PLAY";case"B1":return"KEY_MEDIA_PAUSE";case"B2":return"KEY_MEDIA_RECORD";case"B3":return"KEY_MEDIA_FAST_FORWARD";case"B4":return"KEY_MEDIA_REWIND";case"B5":return"KEY_MEDIA_NEXT_TRACK";case"B6":return"KEY_MEDIA_PREV_TRACK";case"B7":return"KEY_MEDIA_STOP";case"B8":return"KEY_MEDIA_EJECT";case"B9":return"KEY_MEDIA_RANDOM_PLAY";case"CD":return"KEY_MEDIA_PLAY_PAUSE";case"CE":return"KEY_MEDIA_PLAY_SKIP";case"E2":return"KEY_MEDIA_MUTE";case"E9":return"KEY_MEDIA_VOLUME_INC";case"EA":return"KEY_MEDIA_VOLUME_DEC"}return a};
function keyStr_To_keycode(a){switch(a){case"KEY_RESERVED":return"0x00";case"KEY_ERROR_ROLLOVER":return"0x01";case"KEY_POST_FAIL":return"0x02";case"KEY_ERROR_UNDEFINED":return"0x03";case"KEY_A":return"0x04";case"KEY_B":return"0x05";case"KEY_C":return"0x06";case"KEY_D":return"0x07";case"KEY_E":return"0x08";case"KEY_F":return"0x09";case"KEY_G":return"0x0A";case"KEY_H":return"0x0B";case"KEY_I":return"0x0C";case"KEY_J":return"0x0D";case"KEY_K":return"0x0E";case"KEY_L":return"0x0F";case"KEY_M":return"0x10";case"KEY_N":return"0x11";case"KEY_O":return"0x12";case"KEY_P":return"0x13";case"KEY_Q":return"0x14";case"KEY_R":return"0x15";case"KEY_S":return"0x16";case"KEY_T":return"0x17";case"KEY_U":return"0x18";case"KEY_V":return"0x19";case"KEY_W":return"0x1A";case"KEY_X":return"0x1B";case"KEY_Y":return"0x1C";case"KEY_Z":return"0x1D";case"KEY_1":return"0x1E";case"KEY_2":return"0x1F";case"KEY_3":return"0x20";case"KEY_4":return"0x21";case"KEY_5":return"0x22";case"KEY_6":return"0x23";case"KEY_7":return"0x24";case"KEY_8":return"0x25";case"KEY_9":return"0x26";case"KEY_0":return"0x27";case"KEY_ENTER":return"0x28";case"KEY_ESC":return"0x29";case"KEY_BACKSPACE":return"0x2A";case"KEY_TAB":return"0x2B";case"KEY_SPACE":return"0x2C";case"KEY_MINUS":return"0x2D";case"KEY_EQUAL":return"0x2E";case"KEY_LEFT_BRACE":return"0x2F";case"KEY_RIGHT_BRACE":return"0x30";case"KEY_BACKSLASH":return"0x31";case"KEY_NON_US_NUM":return"0x32";case"KEY_SEMICOLON":return"0x33";case"KEY_QUOTE":return"0x34";case"KEY_TILDE":return"0x35";case"KEY_COMMA":return"0x36";case"KEY_PERIOD":return"0x37";case"KEY_SLASH":return"0x38";case"KEY_CAPS_LOCK":return"0x39";case"KEY_CAPSLOCK":return"0x39";case"KEY_F1":return"0x3A";case"KEY_F2":return"0x3B";case"KEY_F3":return"0x3C";case"KEY_F4":return"0x3D";case"KEY_F5":return"0x3E";case"KEY_F6":return"0x3F";case"KEY_F7":return"0x40";case"KEY_F8":return"0x41";case"KEY_F9":return"0x42";case"KEY_F10":return"0x43";case"KEY_F11":return"0x44";case"KEY_F12":return"0x45";case"KEY_PRINTSCREEN":return"0x46";case"KEY_SCROLL_LOCK":return"0x47";case"KEY_SCROLLLOCK":return"0x47";case"KEY_PAUSE":return"0x48";case"KEY_INSERT":return"0x49";case"KEY_HOME":return"0x4A";case"KEY_PAGE_UP":return"0x4B";case"KEY_DELETE":return"0x4C";case"KEY_END":return"0x4D";case"KEY_PAGE_DOWN":return"0x4E";case"KEY_RIGHT_ARROW":return"0x4F";case"KEY_LEFT_ARROW":return"0x50";case"KEY_DOWN_ARROW":return"0x51";case"KEY_UP_ARROW":return"0x52";case"KEY_RIGHT":return"0x4F";case"KEY_LEFT":return"0x50";case"KEY_DOWN":return"0x51";case"KEY_UP":return"0x52";case"KEY_NUM_LOCK":return"0x53";case"KEY_NUMLOCK":return"0x53";case"KEYPAD_DIVIDE":return"0x54";case"KEYPAD_MULTIPLY":return"0x55";case"KEYPAD_SUBTRACT":return"0x56";case"KEYPAD_ADD":return"0x57";case"KEYPAD_ENTER":return"0x58";case"KEYPAD_1":return"0x59";case"KEYPAD_2":return"0x5A";case"KEYPAD_3":return"0x5B";case"KEYPAD_4":return"0x5C";case"KEYPAD_5":return"0x5D";case"KEYPAD_6":return"0x5E";case"KEYPAD_7":return"0x5F";case"KEYPAD_8":return"0x60";case"KEYPAD_9":return"0x61";case"KEYPAD_0":return"0x62";case"KEYPAD_DOT":return"0x63";case"KEY_NON_US":return"0x64";case"KEY_APPLICATION":return"0x65";case"KEY_CONTEXT":return"0x65";case"KEY_MENU":return"0x65";case"KEY_KORENG":return"0x90";case"KEY_CTRL":return"0x67";case"KEY_SHIFT":return"0x68";case"KEY_ALT":return"0x69";case"KEY_GUI":return"0x6A";case"KEY_WINDOWS":return"0x6A";case"KEY_LEFT_CTRL":return"0x67";case"KEY_LEFT_SHIFT":return"0x68";case"KEY_LEFT_ALT":return"0x69";case"KEY_LEFT_GUI":return"0x6A";case"KEY_LEFT_WINDOWS":return"0x6A";case"KEY_RIGHT_CTRL":return"0x6B";case"KEY_RIGHT_SHIFT":return"0x6C";case"KEY_RIGHT_ALT":return"0x6D";case"KEY_RIGHT_GUI":return"0x6E";case"KEY_RIGHT_WINDOWS":return"0x6E";case"KEY_SYSTEM_POWER_DOWN":return"0x81";case"KEY_SYSTEM_SLEEP":return"0x82";case"KEY_SYSTEM_WAKE_UP":return"0x83";case"KEY_MEDIA_PLAY":return"0xB0";case"KEY_MEDIA_PAUSE":return"0xB1";case"KEY_MEDIA_RECORD":return"0xB2";case"KEY_MEDIA_FAST_FORWARD":return"0xB3";case"KEY_MEDIA_REWIND":return"0xB4";case"KEY_MEDIA_NEXT_TRACK":return"0xB5";case"KEY_MEDIA_PREV_TRACK":return"0xB6";case"KEY_MEDIA_STOP":return"0xB7";case"KEY_MEDIA_EJECT":return"0xB8";case"KEY_MEDIA_RANDOM_PLAY":return"0xB9";case"KEY_MEDIA_PLAY_PAUSE":return"0xCD";case"KEY_MEDIA_PLAY_SKIP":return"0xCE";case"KEY_MEDIA_MUTE":return"0xE2";case"KEY_MEDIA_VOLUME_INC":return"0xE9";case"KEY_MEDIA_VOLUME_DEC":return"0xEA"}return a;};
)=====";

/* before html compressor
function eventCode_To_keyStr(eventCode){
	switch(eventCode){
		case 'KeyA':
			return 'KEY_A';
		case 'KeyB':
			return 'KEY_B';
		case 'KeyC':
			return 'KEY_C';
		case 'KeyD':
			return 'KEY_D';
		case 'KeyE':
			return 'KEY_E';
		case 'KeyF':
			return 'KEY_F';
		case 'KeyG':
			return 'KEY_G';
		case 'KeyH':
			return 'KEY_H';
		case 'KeyI':
			return 'KEY_I';
		case 'KeyJ':
			return 'KEY_J';
		case 'KeyK':
			return 'KEY_K';
		case 'KeyL':
			return 'KEY_L';
		case 'KeyM':
			return 'KEY_M';
		case 'KeyN':
			return 'KEY_N';
		case 'KeyO':
			return 'KEY_O';
		case 'KeyP':
			return 'KEY_P';
		case 'KeyQ':
			return 'KEY_Q';
		case 'KeyR':
			return 'KEY_R';
		case 'KeyS':
			return 'KEY_S';
		case 'KeyT':
			return 'KEY_T';
		case 'KeyU':
			return 'KEY_U';
		case 'KeyV':
			return 'KEY_V';
		case 'KeyW':
			return 'KEY_W';
		case 'KeyX':
			return 'KEY_X';
		case 'KeyY':
			return 'KEY_Y';
		case 'KeyZ':
			return 'KEY_Z';
		case 'Digit1':
			return 'KEY_1';
		case 'Digit2':
			return 'KEY_2';
		case 'Digit3':
			return 'KEY_3';
		case 'Digit4':
			return 'KEY_4';
		case 'Digit5':
			return 'KEY_5';
		case 'Digit6':
			return 'KEY_6';
		case 'Digit7':
			return 'KEY_7';
		case 'Digit8':
			return 'KEY_8';
		case 'Digit9':
			return 'KEY_9';
		case 'Digit0':
			return 'KEY_0';
		case 'Enter':
			return 'KEY_ENTER';
		case 'Escape':
			return 'KEY_ESC';
		case 'Backspace':
			return 'KEY_BACKSPACE';
		case 'Tab':
			return 'KEY_TAB';
		case 'Space':
			return 'KEY_SPACE';
		case 'Minus':
			return 'KEY_MINUS';
		case 'Equal':
			return 'KEY_EQUAL';
		case 'BracketLeft':
			return 'KEY_LEFT_BRACE';
		case 'BracketRight':
			return 'KEY_RIGHT_BRACE';
		case 'Backslash':
			return 'KEY_BACKSLASH';
		case 'Semicolon':
			return 'KEY_SEMICOLON';
		case 'Quote':
			return 'KEY_QUOTE';
		case 'Backquote':
			return 'KEY_TILDE';
		case 'Comma':
			return 'KEY_COMMA';
		case 'Period':
			return 'KEY_PERIOD';
		case 'Slash':
			return 'KEY_SLASH';
		case 'CapsLock':
			return 'KEY_CAPS_LOCK';
		case 'F1':
			return 'KEY_F1';
		case 'F2':
			return 'KEY_F2';
		case 'F3':
			return 'KEY_F3';
		case 'F4':
			return 'KEY_F4';
		case 'F5':
			return 'KEY_F5';
		case 'F6':
			return 'KEY_F6';
		case 'F7':
			return 'KEY_F7';
		case 'F8':
			return 'KEY_F8';
		case 'F9':
			return 'KEY_F9';
		case 'F10':
			return 'KEY_F10';
		case 'F11':
			return 'KEY_F11';
		case 'F12':
			return 'KEY_F12';
		case 'PrintScreen':
			return 'KEY_PRINTSCREEN';
		case 'ScrollLock':
			return 'KEY_SCROLL_LOCK';
		case 'Pause':
			return 'KEY_PAUSE';
		case 'Insert':
			return 'KEY_INSERT';
		case 'Home':
			return 'KEY_HOME';
		case 'PageUp':
			return 'KEY_PAGE_UP';
		case 'Delete':
			return 'KEY_DELETE';
		case 'End':
			return 'KEY_END';
		case 'PageDown':
			return 'KEY_PAGE_DOWN';
		case 'ArrowRight':
			return 'KEY_RIGHT';
		case 'ArrowLeft':
			return 'KEY_LEFT';
		case 'ArrowDown':
			return 'KEY_DOWN';
		case 'ArrowUp':
			return 'KEY_UP';
		case 'NumLock':
			return 'KEY_NUM_LOCK';
		case 'NumpadDivide':
			return 'KEYPAD_DIVIDE';
		case 'NumpadMultiply':
			return 'KEYPAD_MULTIPLY';
		case 'NumpadSubtract':
			return 'KEYPAD_SUBTRACT';
		case 'NumpadAdd':
			return 'KEYPAD_ADD';
		case 'NumpadEnter':
			return 'KEYPAD_ENTER';
		case 'Numpad1':
			return 'KEYPAD_1';
		case 'Numpad2':
			return 'KEYPAD_2';
		case 'Numpad3':
			return 'KEYPAD_3';
		case 'Numpad4':
			return 'KEYPAD_4';
		case 'Numpad5':
			return 'KEYPAD_5';
		case 'Numpad6':
			return 'KEYPAD_6';
		case 'Numpad7':
			return 'KEYPAD_7';
		case 'Numpad8':
			return 'KEYPAD_8';
		case 'Numpad9':
			return 'KEYPAD_9';
		case 'Numpad0':
			return 'KEYPAD_0';
		case 'NumpadDecimal':
			return 'KEYPAD_PERIOD';
		case 'ControlLeft':
			return 'KEY_LEFT_CTRL';
		case 'ShiftLeft':
			return 'KEY_LEFT_SHIFT';
		case 'AltLeft':
			return 'KEY_LEFT_ALT';
		case 'MetaLeft':
			return 'KEY_LEFT_GUI';
		case 'ControlRight':
			return 'KEY_RIGHT_CTRL';
		case 'ShiftRight':
			return 'KEY_RIGHT_SHIFT';
		case 'AltRight':
			return 'KEY_RIGHT_ALT';
		//case 'MetaRight':
		//	return 'KEY_RIGHT_GUI';
		case 'ContextMenu':
			return 'KEY_CONTEXT';
	}
}
function eventCode_To_viewId(eventCode){
	switch(eventCode){
		case 'KeyA':
			return '#key_a';
		case 'KeyB':
			return '#key_b';
		case 'KeyC':
			return '#key_c';
		case 'KeyD':
			return '#key_d';
		case 'KeyE':
			return '#key_e';
		case 'KeyF':
			return '#key_f';
		case 'KeyG':
			return '#key_g';
		case 'KeyH':
			return '#key_h';
		case 'KeyI':
			return '#key_i';
		case 'KeyJ':
			return '#key_j';
		case 'KeyK':
			return '#key_k';
		case 'KeyL':
			return '#key_l';
		case 'KeyM':
			return '#key_m';
		case 'KeyN':
			return '#key_n';
		case 'KeyO':
			return '#key_o';
		case 'KeyP':
			return '#key_p';
		case 'KeyQ':
			return '#key_q';
		case 'KeyR':
			return '#key_r';
		case 'KeyS':
			return '#key_s';
		case 'KeyT':
			return '#key_t';
		case 'KeyU':
			return '#key_u';
		case 'KeyV':
			return '#key_v';
		case 'KeyW':
			return '#key_w';
		case 'KeyX':
			return '#key_x';
		case 'KeyY':
			return '#key_y';
		case 'KeyZ':
			return '#key_z';
		case 'Digit1':
			return '#key_one';
		case 'Digit2':
			return '#key_two';
		case 'Digit3':
			return '#key_three';
		case 'Digit4':
			return '#key_four';
		case 'Digit5':
			return '#key_five';
		case 'Digit6':
			return '#key_six';
		case 'Digit7':
			return '#key_seven';
		case 'Digit8':
			return '#key_eight';
		case 'Digit9':
			return '#key_nine';
		case 'Digit0':
			return '#key_zero';
		case 'Enter':
			return '#key_enter';
		case 'Escape':
			return '#key_esc';
		case 'Backspace':
			return '#key_backspace';
		case 'Tab':
			return '#key_tab';
		case 'Space':
			return '#key_space';
		case 'Minus':
			return '#key_hyphen';
		case 'Equal':
			return '#key_equals';
		case 'BracketLeft':
			return '#key_left_bracket';
		case 'BracketRight':
			return '#key_right_bracket';
		case 'Backslash':
			return '#key_backslash';
		case 'Semicolon':
			return '#key_semicolon';
		case 'Quote':
			return '#key_apostrophe';
		case 'Backquote':
			return '#key_accent';
		case 'Comma':
			return '#key_comma';
		case 'Period':
			return '#key_period';
		case 'Slash':
			return '#key_forwardslash';
		case 'CapsLock':
			return '#key_caps_lock';
		case 'F1':
			return '#key_f1';
		case 'F2':
			return '#key_f2';
		case 'F3':
			return '#key_f3';
		case 'F4':
			return '#key_f4';
		case 'F5':
			return '#key_f5';
		case 'F6':
			return '#key_f6';
		case 'F7':
			return '#key_f7';
		case 'F8':
			return '#key_f8';
		case 'F9':
			return '#key_f9';
		case 'F10':
			return '#key_f10';
		case 'F11':
			return '#key_f11';
		case 'F12':
			return '#key_f12';
		case 'PrintScreen':
			return '#key_print';
		case 'ScrollLock':
			return '#key_scroll_lock';
		case 'Pause':
			return '#key_pause_break';
		case 'Insert':
			return '#key_insert';
		case 'Home':
			return '#key_home';
		case 'PageUp':
			return '#key_page_up';
		case 'Delete':
			return '#key_delete';
		case 'End':
			return '#key_end';
		case 'PageDown':
			return '#key_page_down';
		case 'ArrowRight':
			return '#key_right';
		case 'ArrowLeft':
			return '#key_left';
		case 'ArrowDown':
			return '#key_down';
		case 'ArrowUp':
			return '#key_up';
		case 'NumLock':
			return '#key_num_lock';
		case 'NumpadDivide':
			return '#key_divide';
		case 'NumpadMultiply':
			return '#key_multiply';
		case 'NumpadSubtract':
			return '#key_subtract';
		case 'NumpadAdd':
			return '#key_add';
		case 'NumpadEnter':
			return '#key_num_enter';
		case 'Numpad1':
			return '#key_num_1';
		case 'Numpad2':
			return '#key_num_2';
		case 'Numpad3':
			return '#key_num_3';
		case 'Numpad4':
			return '#key_num_4';
		case 'Numpad5':
			return '#key_num_5';
		case 'Numpad6':
			return '#key_num_6';
		case 'Numpad7':
			return '#key_num_7';
		case 'Numpad8':
			return '#key_num_8';
		case 'Numpad9':
			return '#key_num_9';
		case 'Numpad0':
			return '#key_num_0';
		case 'NumpadDecimal':
			return '#key_num_decimal';
		case 'ControlLeft':
			return '#key_left_ctrl';
		case 'ShiftLeft':
			return '#key_left_shift';
		case 'AltLeft':
			return '#key_left_alt';
		case 'MetaLeft':
			return '#key_left_gui';
		case 'ControlRight':
			return '#key_right_ctrl';
		case 'ShiftRight':
			return '#key_right_shift';
		case 'AltRight':
			return '#key_right_alt';
		//case 'MetaRight':
		//	return '#key_right_gui';
		case 'ContextMenu':
			return '#key_right_app';
	}
}
function viewId_To_keyStr(viewId){
	switch(viewId){
		case 'key_a':
			return 'KEY_A';
		case 'key_b':
			return 'KEY_B';
		case 'key_c':
			return 'KEY_C';
		case 'key_d':
			return 'KEY_D';
		case 'key_e':
			return 'KEY_E';
		case 'key_f':
			return 'KEY_F';
		case 'key_g':
			return 'KEY_G';
		case 'key_h':
			return 'KEY_H';
		case 'key_i':
			return 'KEY_I';
		case 'key_j':
			return 'KEY_J';
		case 'key_k':
			return 'KEY_K';
		case 'key_l':
			return 'KEY_L';
		case 'key_m':
			return 'KEY_M';
		case 'key_n':
			return 'KEY_N';
		case 'key_o':
			return 'KEY_O';
		case 'key_p':
			return 'KEY_P';
		case 'key_q':
			return 'KEY_Q';
		case 'key_r':
			return 'KEY_R';
		case 'key_s':
			return 'KEY_S';
		case 'key_t':
			return 'KEY_T';
		case 'key_u':
			return 'KEY_U';
		case 'key_v':
			return 'KEY_V';
		case 'key_w':
			return 'KEY_W';
		case 'key_x':
			return 'KEY_X';
		case 'key_y':
			return 'KEY_Y';
		case 'key_z':
			return 'KEY_Z';
		case 'key_one':
			return 'KEY_1';
		case 'key_two':
			return 'KEY_2';
		case 'key_three':
			return 'KEY_3';
		case 'key_four':
			return 'KEY_4';
		case 'key_five':
			return 'KEY_5';
		case 'key_six':
			return 'KEY_6';
		case 'key_seven':
			return 'KEY_7';
		case 'key_eight':
			return 'KEY_8';
		case 'key_nine':
			return 'KEY_9';
		case 'key_zero':
			return 'KEY_0';
		case 'key_enter':
			return 'KEY_ENTER';
		case 'key_esc':
			return 'KEY_ESC';
		case 'key_backspace':
			return 'KEY_BACKSPACE';
		case 'key_tab':
			return 'KEY_TAB';
		case 'key_space':
			return 'KEY_SPACE';
		case 'key_hyphen':
			return 'KEY_MINUS';
		case 'key_equals':
			return 'KEY_EQUAL';
		case 'key_left_bracket':
			return 'KEY_LEFT_BRACE';
		case 'key_right_bracket':
			return 'KEY_RIGHT_BRACE';
		case 'key_backslash':
			return 'KEY_BACKSLASH';
		case 'key_semicolon':
			return 'KEY_SEMICOLON';
		case 'key_apostrophe':
			return 'KEY_QUOTE';
		case 'key_accent':
			return 'KEY_TILDE';
		case 'key_comma':
			return 'KEY_COMMA';
		case 'key_period':
			return 'KEY_PERIOD';
		case 'key_forwardslash':
			return 'KEY_SLASH';
		case 'key_caps_lock':
			return 'KEY_CAPS_LOCK';
		case 'key_f1':
			return 'KEY_F1';
		case 'key_f2':
			return 'KEY_F2';
		case 'key_f3':
			return 'KEY_F3';
		case 'key_f4':
			return 'KEY_F4';
		case 'key_f5':
			return 'KEY_F5';
		case 'key_f6':
			return 'KEY_F6';
		case 'key_f7':
			return 'KEY_F7';
		case 'key_f8':
			return 'KEY_F8';
		case 'key_f9':
			return 'KEY_F9';
		case 'key_f10':
			return 'KEY_F10';
		case 'key_f11':
			return 'KEY_F11';
		case 'key_f12':
			return 'KEY_F12';
		case 'key_print':
			return 'KEY_PRINTSCREEN';
		case 'key_scroll_lock':
			return 'KEY_SCROLL_LOCK';
		case 'key_pause_break':
			return 'KEY_PAUSE';
		case 'key_insert':
			return 'KEY_INSERT';
		case 'key_home':
			return 'KEY_HOME';
		case 'key_page_up':
			return 'KEY_PAGE_UP';
		case 'key_delete':
			return 'KEY_DELETE';
		case 'key_end':
			return 'KEY_END';
		case 'key_page_down':
			return 'KEY_PAGE_DOWN';
		case 'key_right':
			return 'KEY_RIGHT';
		case 'key_left':
			return 'KEY_LEFT';
		case 'key_down':
			return 'KEY_DOWN';
		case 'key_up':
			return 'KEY_UP';
		case 'key_num_lock':
			return 'KEY_NUM_LOCK';
		case 'key_divide':
			return 'KEYPAD_DIVIDE';
		case 'key_multiply':
			return 'KEYPAD_MULTIPLY';
		case 'key_subtract':
			return 'KEYPAD_SUBTRACT';
		case 'key_add':
			return 'KEYPAD_ADD';
		case 'key_num_enter':
			return 'KEYPAD_ENTER';
		case 'key_num_1':
			return 'KEYPAD_1';
		case 'key_num_2':
			return 'KEYPAD_2';
		case 'key_num_3':
			return 'KEYPAD_3';
		case 'key_num_4':
			return 'KEYPAD_4';
		case 'key_num_5':
			return 'KEYPAD_5';
		case 'key_num_6':
			return 'KEYPAD_6';
		case 'key_num_7':
			return 'KEYPAD_7';
		case 'key_num_8':
			return 'KEYPAD_8';
		case 'key_num_9':
			return 'KEYPAD_9';
		case 'key_num_0':
			return 'KEYPAD_0';
		case 'key_num_decimal':
			return 'KEYPAD_PERIOD';
		case 'key_left_ctrl':
			return 'KEY_LEFT_CTRL';
		case 'key_left_shift':
			return 'KEY_LEFT_SHIFT';
		case 'key_left_alt':
			return 'KEY_LEFT_ALT';
		case 'key_left_gui':
			return 'KEY_LEFT_GUI';
		case 'key_right_ctrl':
			return 'KEY_RIGHT_CTRL';
		case 'key_right_shift':
			return 'KEY_RIGHT_SHIFT';
		case 'key_right_alt':
			return 'KEY_RIGHT_ALT';
		case 'key_right_app':
			return 'KEY_CONTEXT';
	}
}
function keycode_To_keyStr(keycode){
	switch(keycode.slice(2)){
		case '00':
			return 'KEY_RESERVED';
		case '01':
			return 'KEY_ERROR_ROLLOVER';
		case '02':
			return 'KEY_POST_FAIL';
		case '03':
			return 'KEY_ERROR_UNDEFINED';
		case '04':
			return 'KEY_A';
		case '05':
			return 'KEY_B';
		case '06':
			return 'KEY_C';
		case '07':
			return 'KEY_D';
		case '08':
			return 'KEY_E';
		case '09':
			return 'KEY_F';
		case '0A':
			return 'KEY_G';
		case '0B':
			return 'KEY_H';
		case '0C':
			return 'KEY_I';
		case '0D':
			return 'KEY_J';
		case '0E':
			return 'KEY_K';
		case '0F':
			return 'KEY_L';
		case '10':
			return 'KEY_M';
		case '11':
			return 'KEY_N';
		case '12':
			return 'KEY_O';
		case '13':
			return 'KEY_P';
		case '14':
			return 'KEY_Q';
		case '15':
			return 'KEY_R';
		case '16':
			return 'KEY_S';
		case '17':
			return 'KEY_T';
		case '18':
			return 'KEY_U';
		case '19':
			return 'KEY_V';
		case '1A':
			return 'KEY_W';
		case '1B':
			return 'KEY_X';
		case '1C':
			return 'KEY_Y';
		case '1D':
			return 'KEY_Z';
		case '1E':
			return 'KEY_1';
		case '1F':
			return 'KEY_2';
		case '20':
			return 'KEY_3';
		case '21':
			return 'KEY_4';
		case '22':
			return 'KEY_5';
		case '23':
			return 'KEY_6';
		case '24':
			return 'KEY_7';
		case '25':
			return 'KEY_8';
		case '26':
			return 'KEY_9';
		case '27':
			return 'KEY_0';
		case '28':
			return 'KEY_ENTER';
		case '29':
			return 'KEY_ESC';
		case '2A':
			return 'KEY_BACKSPACE';
		case '2B':
			return 'KEY_TAB';
		case '2C':
			return 'KEY_SPACE';
		case '2D':
			return 'KEY_MINUS';
		case '2E':
			return 'KEY_EQUAL';
		case '2F':
			return 'KEY_LEFT_BRACE';
		case '30':
			return 'KEY_RIGHT_BRACE';
		case '31':
			return 'KEY_BACKSLASH';
		case '32':
			return 'KEY_NON_US_NUM';
		case '33':
			return 'KEY_SEMICOLON';
		case '34':
			return 'KEY_QUOTE';
		case '35':
			return 'KEY_TILDE';
		case '36':
			return 'KEY_COMMA';
		case '37':
			return 'KEY_PERIOD';
		case '38':
			return 'KEY_SLASH';
		case '39':
			return 'KEY_CAPS_LOCK';
		case '3A':
			return 'KEY_F1';
		case '3B':
			return 'KEY_F2';
		case '3C':
			return 'KEY_F3';
		case '3D':
			return 'KEY_F4';
		case '3E':
			return 'KEY_F5';
		case '3F':
			return 'KEY_F6';
		case '40':
			return 'KEY_F7';
		case '41':
			return 'KEY_F8';
		case '42':
			return 'KEY_F9';
		case '43':
			return 'KEY_F10';
		case '44':
			return 'KEY_F11';
		case '45':
			return 'KEY_F12';
		case '46':
			return 'KEY_PRINTSCREEN';
		case '47':
			return 'KEY_SCROLL_LOCK';
		case '48':
			return 'KEY_PAUSE';
		case '49':
			return 'KEY_INSERT';
		case '4A':
			return 'KEY_HOME';
		case '4B':
			return 'KEY_PAGE_UP';
		case '4C':
			return 'KEY_DELETE';
		case '4D':
			return 'KEY_END';
		case '4E':
			return 'KEY_PAGE_DOWN';
		case '4F':
			return 'KEY_RIGHT';
		case '50':
			return 'KEY_LEFT';
		case '51':
			return 'KEY_DOWN';
		case '52':
			return 'KEY_UP';
		case '53':
			return 'KEY_NUM_LOCK';
		case '54':
			return 'KEYPAD_DIVIDE';
		case '55':
			return 'KEYPAD_MULTIPLY';
		case '56':
			return 'KEYPAD_SUBTRACT';
		case '57':
			return 'KEYPAD_ADD';
		case '58':
			return 'KEYPAD_ENTER';
		case '59':
			return 'KEYPAD_1';
		case '5A':
			return 'KEYPAD_2';
		case '5B':
			return 'KEYPAD_3';
		case '5C':
			return 'KEYPAD_4';
		case '5D':
			return 'KEYPAD_5';
		case '5E':
			return 'KEYPAD_6';
		case '5F':
			return 'KEYPAD_7';
		case '60':
			return 'KEYPAD_8';
		case '61':
			return 'KEYPAD_9';
		case '62':
			return 'KEYPAD_0';
		case '63':
			return 'KEYPAD_DOT';
		case '64':
			return 'KEY_NON_US';
		case '65':
			return 'KEY_APPLICATION';

		case '90':
			return 'KEY_KORENG';

		case '67':
			return 'KEY_LEFT_CTRL';
		case '68':
			return 'KEY_LEFT_SHIFT';
		case '69':
			return 'KEY_LEFT_ALT';
		case '6A':
			return 'KEY_LEFT_GUI';
		case '6B':
			return 'KEY_RIGHT_CTRL';
		case '6C':
			return 'KEY_RIGHT_SHIFT';
		case '6D':
			return 'KEY_RIGHT_ALT';
		case '6E':
			return 'KEY_RIGHT_GUI';

		case '81':
			return 'KEY_SYSTEM_POWER_DOWN';
		case '82':
			return 'KEY_SYSTEM_SLEEP';
		case '83':
			return 'KEY_SYSTEM_WAKE_UP';

		case 'B0':
			return 'KEY_MEDIA_PLAY';
		case 'B1':
			return 'KEY_MEDIA_PAUSE';
		case 'B2':
			return 'KEY_MEDIA_RECORD';
		case 'B3':
			return 'KEY_MEDIA_FAST_FORWARD';
		case 'B4':
			return 'KEY_MEDIA_REWIND';
		case 'B5':
			return 'KEY_MEDIA_NEXT_TRACK';
		case 'B6':
			return 'KEY_MEDIA_PREV_TRACK';
		case 'B7':
			return 'KEY_MEDIA_STOP';
		case 'B8':
			return 'KEY_MEDIA_EJECT';
		case 'B9':
			return 'KEY_MEDIA_RANDOM_PLAY';
		case 'CD':
			return 'KEY_MEDIA_PLAY_PAUSE';
		case 'CE':
			return 'KEY_MEDIA_PLAY_SKIP';
		case 'E2':
			return 'KEY_MEDIA_MUTE';
		case 'E9':
			return 'KEY_MEDIA_VOLUME_INC';
		case 'EA':
			return 'KEY_MEDIA_VOLUME_DEC';
	}
    return keycode;
}
function keyStr_To_keycode(keyStr){
	switch(keyStr){
		case 'KEY_RESERVED':
			return '0x00';
		case 'KEY_ERROR_ROLLOVER':
			return '0x01';
		case 'KEY_POST_FAIL':
			return '0x02';
		case 'KEY_ERROR_UNDEFINED':
			return '0x03';
		case 'KEY_A':
			return '0x04';
		case 'KEY_B':
			return '0x05';
		case 'KEY_C':
			return '0x06';
		case 'KEY_D':
			return '0x07';
		case 'KEY_E':
			return '0x08';
		case 'KEY_F':
			return '0x09';
		case 'KEY_G':
			return '0x0A';
		case 'KEY_H':
			return '0x0B';
		case 'KEY_I':
			return '0x0C';
		case 'KEY_J':
			return '0x0D';
		case 'KEY_K':
			return '0x0E';
		case 'KEY_L':
			return '0x0F';
		case 'KEY_M':
			return '0x10';
		case 'KEY_N':
			return '0x11';
		case 'KEY_O':
			return '0x12';
		case 'KEY_P':
			return '0x13';
		case 'KEY_Q':
			return '0x14';
		case 'KEY_R':
			return '0x15';
		case 'KEY_S':
			return '0x16';
		case 'KEY_T':
			return '0x17';
		case 'KEY_U':
			return '0x18';
		case 'KEY_V':
			return '0x19';
		case 'KEY_W':
			return '0x1A';
		case 'KEY_X':
			return '0x1B';
		case 'KEY_Y':
			return '0x1C';
		case 'KEY_Z':
			return '0x1D';
		case 'KEY_1':
			return '0x1E';
		case 'KEY_2':
			return '0x1F';
		case 'KEY_3':
			return '0x20';
		case 'KEY_4':
			return '0x21';
		case 'KEY_5':
			return '0x22';
		case 'KEY_6':
			return '0x23';
		case 'KEY_7':
			return '0x24';
		case 'KEY_8':
			return '0x25';
		case 'KEY_9':
			return '0x26';
		case 'KEY_0':
			return '0x27';
		case 'KEY_ENTER':
			return '0x28';
		case 'KEY_ESC':
			return '0x29';
		case 'KEY_BACKSPACE':
			return '0x2A';
		case 'KEY_TAB':
			return '0x2B';
		case 'KEY_SPACE':
			return '0x2C';
		case 'KEY_MINUS':
			return '0x2D';
		case 'KEY_EQUAL':
			return '0x2E';
		case 'KEY_LEFT_BRACE':
			return '0x2F';
		case 'KEY_RIGHT_BRACE':
			return '0x30';
		case 'KEY_BACKSLASH':
			return '0x31';
		case 'KEY_NON_US_NUM':
			return '0x32';
		case 'KEY_SEMICOLON':
			return '0x33';
		case 'KEY_QUOTE':
			return '0x34';
		case 'KEY_TILDE':
			return '0x35';
		case 'KEY_COMMA':
			return '0x36';
		case 'KEY_PERIOD':
			return '0x37';
		case 'KEY_SLASH':
			return '0x38';
		case 'KEY_CAPS_LOCK':
			return '0x39';
		case 'KEY_CAPSLOCK':
			return '0x39';
		case 'KEY_F1':
			return '0x3A';
		case 'KEY_F2':
			return '0x3B';
		case 'KEY_F3':
			return '0x3C';
		case 'KEY_F4':
			return '0x3D';
		case 'KEY_F5':
			return '0x3E';
		case 'KEY_F6':
			return '0x3F';
		case 'KEY_F7':
			return '0x40';
		case 'KEY_F8':
			return '0x41';
		case 'KEY_F9':
			return '0x42';
		case 'KEY_F10':
			return '0x43';
		case 'KEY_F11':
			return '0x44';
		case 'KEY_F12':
			return '0x45';
		case 'KEY_PRINTSCREEN':
			return '0x46';
		case 'KEY_SCROLL_LOCK':
			return '0x47';
		case 'KEY_SCROLLLOCK':
			return '0x47';
		case 'KEY_PAUSE':
			return '0x48';
		case 'KEY_INSERT':
			return '0x49';
		case 'KEY_HOME':
			return '0x4A';
		case 'KEY_PAGE_UP':
			return '0x4B';
		case 'KEY_DELETE':
			return '0x4C';
		case 'KEY_END':
			return '0x4D';
		case 'KEY_PAGE_DOWN':
			return '0x4E';
		case 'KEY_RIGHT_ARROW':
			return '0x4F';
		case 'KEY_LEFT_ARROW':
			return '0x50';
		case 'KEY_DOWN_ARROW':
			return '0x51';
		case 'KEY_UP_ARROW':
			return '0x52';
		case 'KEY_RIGHT':
			return '0x4F';
		case 'KEY_LEFT':
			return '0x50';
		case 'KEY_DOWN':
			return '0x51';
		case 'KEY_UP':
			return '0x52';
		case 'KEY_NUM_LOCK':
			return '0x53';
		case 'KEY_NUMLOCK':
			return '0x53';
		case 'KEYPAD_DIVIDE':
			return '0x54';
		case 'KEYPAD_MULTIPLY':
			return '0x55';
		case 'KEYPAD_SUBTRACT':
			return '0x56';
		case 'KEYPAD_ADD':
			return '0x57';
		case 'KEYPAD_ENTER':
			return '0x58';
		case 'KEYPAD_1':
			return '0x59';
		case 'KEYPAD_2':
			return '0x5A';
		case 'KEYPAD_3':
			return '0x5B';
		case 'KEYPAD_4':
			return '0x5C';
		case 'KEYPAD_5':
			return '0x5D';
		case 'KEYPAD_6':
			return '0x5E';
		case 'KEYPAD_7':
			return '0x5F';
		case 'KEYPAD_8':
			return '0x60';
		case 'KEYPAD_9':
			return '0x61';
		case 'KEYPAD_0':
			return '0x62';
		case 'KEYPAD_DOT':
			return '0x63';
		case 'KEY_NON_US':
			return '0x64';
		case 'KEY_APPLICATION':
			return '0x65';
		case 'KEY_CONTEXT':
			return '0x65';
		case 'KEY_MENU':
			return '0x65';

		case 'KEY_KORENG':
			return '0x90';

		case 'KEY_CTRL':
			return '0x67';
		case 'KEY_SHIFT':
			return '0x68';
		case 'KEY_ALT':
			return '0x69';
		case 'KEY_GUI':
			return '0x6A';
		case 'KEY_WINDOWS':
			return '0x6A';
		case 'KEY_LEFT_CTRL':
			return '0x67';
		case 'KEY_LEFT_SHIFT':
			return '0x68';
		case 'KEY_LEFT_ALT':
			return '0x69';
		case 'KEY_LEFT_GUI':
			return '0x6A';
		case 'KEY_LEFT_WINDOWS':
			return '0x6A';
		case 'KEY_RIGHT_CTRL':
			return '0x6B';
		case 'KEY_RIGHT_SHIFT':
			return '0x6C';
		case 'KEY_RIGHT_ALT':
			return '0x6D';
		case 'KEY_RIGHT_GUI':
			return '0x6E';
		case 'KEY_RIGHT_WINDOWS':
			return '0x6E';

		case 'KEY_SYSTEM_POWER_DOWN':
			return '0x81';
		case 'KEY_SYSTEM_SLEEP':
			return '0x82';
		case 'KEY_SYSTEM_WAKE_UP':
			return '0x83';

		case 'KEY_MEDIA_PLAY':
			return '0xB0';
		case 'KEY_MEDIA_PAUSE':
			return '0xB1';
		case 'KEY_MEDIA_RECORD':
			return '0xB2';
		case 'KEY_MEDIA_FAST_FORWARD':
			return '0xB3';
		case 'KEY_MEDIA_REWIND':
			return '0xB4';
		case 'KEY_MEDIA_NEXT_TRACK':
			return '0xB5';
		case 'KEY_MEDIA_PREV_TRACK':
			return '0xB6';
		case 'KEY_MEDIA_STOP':
			return '0xB7';
		case 'KEY_MEDIA_EJECT':
			return '0xB8';
		case 'KEY_MEDIA_RANDOM_PLAY':
			return '0xB9';
		case 'KEY_MEDIA_PLAY_PAUSE':
			return '0xCD';
		case 'KEY_MEDIA_PLAY_SKIP':
			return '0xCE';
		case 'KEY_MEDIA_MUTE':
			return '0xE2';
		case 'KEY_MEDIA_VOLUME_INC':
			return '0xE9';
		case 'KEY_MEDIA_VOLUME_DEC':
			return '0xEA';
	}
    return keyStr;
}
*/