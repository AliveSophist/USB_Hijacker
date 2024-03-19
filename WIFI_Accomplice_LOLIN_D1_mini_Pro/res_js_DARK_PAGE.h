const char raw_js_DARK_PAGE[] PROGMEM =
R"=====(
/**⦓ SCRIPT, ABOUT LIBRARIES ⦔**/

// Split.js configuration
document.querySelector("#divPanelL").parentNode.classList.add('split');
Split(['#divPanelL', '#divPanelR'], { minSize: 0, gutterSize: 18, });

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
                        return `<span class="btnPullFile gradient-animation-text">${data}</span>`;
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

        if(rowKEYCODE) $("#tbDirectory tbody").prepend( rowKEYCODE );   // always 2nd row
        if(rowMAPPER) $("#tbDirectory tbody").prepend( rowMAPPER );     // always 1st row
    });
    dtDirectory.order([0, 'asc']).draw();
}
function findRow(filename)
{
    const arrTrNodes = [...document.querySelectorAll("#tbDirectory tr")];
    return arrTrNodes.find(row => row.textContent.includes(filename));
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

// contextMenu.min.js (https://www.cssscript.com/lightweight-custom-context-menu/)
const contextMenu=function(){let c={};function t(e){c=e;let t=document.createElement("div");t.classList.add(c.className||"contextMenu"),t.setAttribute("id",c.id||"contextMenuId"),o(t,{position:"fixed",display:"none",top:"0px",left:"0px"});let i=document.createElement("ul");o(i,{listStyle:"none",padding:"0px",margin:"0px",display:"flex",flexDirection:"column"}),c.items.forEach((t,n)=>{let e=document.createElement("li");e.innerHTML=t.template,e.classList.add("contextMenuItem"),o(e,{cursor:"pointer"}),e.addEventListener("click",function(e){t.onClick(e.target,n)}),i.classList.add("contextMenuList"),i.appendChild(e)}),t.appendChild(i),document.body.appendChild(t)}function o(t,n){Object.keys(n).forEach(e=>{t.style[e]=n[e]})}return t.prototype.init=function(e){let t=e||document;e=c.id||"contextMenuId";let n=document.querySelector("#"+e);n.addEventListener("mouseleave",function(){o(this,{display:"none"})}),t.addEventListener("contextmenu",function(e){e.preventDefault(),o(n,{top:e.clientY+"px",left:e.clientX+"px",display:"block"})})},function(e){return new t(e)}}();
function initializeContextMenu()
{
    contextMenu(
    {
        items:
        [
            {	template: "DELETE",
                onClick : function()
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
            {	template: "RENAME",
                onClick : function()
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
        ]
    }).init(document.querySelector('#tbDirectory').querySelector('tbody'));
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
            content: tooltipThing.replace(/\n/g, "<br/>"),
            placement: 'right',
            allowHTML: true,
            theme: 'light'
        });

        return this;
    }
}

const taFileContent = document.querySelector("#taFileContent");
const divFileSelector = document.querySelector("#divFileSelector");
const btnCreateFile = document.querySelector("#btnCreateFile");
const btnSaveFile = document.querySelector("#btnSaveFile");

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

    isEnabledTextarea = false;
}
disableTextarea();

function focusSelector(filename)
{
    for(var selector of divFileSelector.querySelectorAll('span'))
        selector.classList = (selector.dataset.filename === filename) ? 'fileSelected' : '';
}
function createSelector(filename)
{
    const selector = document.createElement('span');
    selector.dataset.filename = filename;
    selector.innerText = (filename.lastIndexOf('.') > -1) ? filename.substring(0, filename.lastIndexOf('.')) : filename;
    selector.addEventListener("click", function()
    {
        disableTextarea();

        if(getCurrentFilename() === this.dataset.filename) {
            enableTextarea();
            return;
        };

        setCurrentFilename(this.dataset.filename);
        let filedata = getFiledata(this.dataset.filename);

        enableTextarea(filedata);
        focusSelector(this.dataset.filename);
    });

    divFileSelector.appendChild(selector);
    divFileSelector.style.display = 'block';
}
function deleteSelector(filename)
{
    if(getCurrentFilename() === filename) {
        setCurrentFilename('');
        taFileContent.value = '';

        focusSelector('');
    }

    for(var selector of divFileSelector.querySelectorAll('span'))
        if(selector.dataset.filename === filename)
            selector.remove();

    if(divFileSelector.querySelectorAll('span').length == 0)
        divFileSelector.style.display = 'none';
}



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
        /* data: nothing, */
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
        /* data: nothing, */
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
        if(isProcessing)
            return;


        if (resultCombined.includes('7R4N5M15510N_C0MPL373D') || resultCombined.includes('3RR0R3D'))
        {
            clearInterval(run);

            console.log(resultCombined);
            TerminalWriter.addTextInstantly('<br/>REQUEST FINISH');


            // Draw Contents
            {
                let rows = resultCombined.split('\n');
                for (var row of rows) {
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

let mapFiledata = new Map();
function isExistFiledata(filename)      { return mapFiledata.has(filename); }
function setFiledata(filename, filedata)
{
    if(filename.length < 1)
        return;

    if (!isExistFiledata(filename))
        createSelector(filename);

    mapFiledata.set(filename, filedata);
}
function getFiledata(filename)
{
    return mapFiledata.get(filename);
}
function delFiledata(filename)
{
    if(!isExistFiledata(filename))
        return;

    deleteSelector(filename);
    mapFiledata.delete(filename);
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
    focusSelector('');


    SEND_REQUEST(`PULL_FILE("${filename}")`);

    run = setInterval(() =>
    {
        if(isProcessing)
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
                    focusSelector(getCurrentFilename());
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

    for (let i = 0; i < text.length; i++) {
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


    dataSplited = splitTextByBytes(taFileContent.value, 512);
    dataSplited.push('7R4N5M15510N_C0MPL373D');

    $('#tbDirectory').hide();
    $('#divTerminal').show();
    disableTextarea();


    SEND_REQUEST(`PUSH_FILE("${filename}")`);

    run = setInterval(() =>
    {
        if(isProcessing)
            return;


        if (dataSplited.length == 0)
        {
            if(isProcessing)
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
                if(isJustCreateFile)
                {
                    createRow(getCurrentFilename(), 0);

                    enableTextarea('');
                    focusSelector(getCurrentFilename());
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


    if(strParam == null)
        SEND_REQUEST(strRequest);
    else
    if(!Array.isArray(strParam))
        SEND_REQUEST(`${strRequest}("${strParam}")`);
    else {
        let strParamFormatted = strParam.map(param => `"${param}"`).join('|');
        SEND_REQUEST(`${strRequest}(${strParamFormatted})`);
    }

    run = setInterval(() =>
    {
        if(isProcessing)
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
                if(strRequest.includes('DELETE_FILE'))
                {
                    let filename = strParam;

                    deleteRow(filename);
                    delFiledata(filename);
                }
                if(strRequest.includes('RENAME_FILE'))
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

$( document ).ready(function()
{
    TerminalWriter.addTextWithAnimation('Please Wait . . .');

    // LOAD tbDirectory contents!
    executePullRootData();

    // Initialize Libraries
    initializeDataTable();
    initializeContextMenu();



    // divPanelL can be swap its content by wheel up/down
    document.querySelector("#divPanelL").addEventListener('wheel', function(event)
    {
        if (isRunning != 0)
            return;

        const currentChildIndex = (() =>
        {
            for (var i = 0; i < this.children.length; i++) {
                if (this.children[i].style.display !== 'none')
                    return i;
            }

            return 0;
        })();
        let currentChildNode  = this.children[currentChildIndex];

        if (event.deltaY < 0 && currentChildIndex != 0)
        {
            /* when wheel down evnets, contitions before swapping */
            {
                // Only through thead, tbDirectory can be swapped
                if( currentChildNode.contains(tbDirectory) ) {
                    if(!event.target.closest('thead'))
                        return;
                }
            }

            $(currentChildNode).hide();
            $(currentChildNode = this.children[currentChildIndex-1]).show();
            currentChildNode.scrollTop = 0;

            /* when wheel down evnets, contitions after swapping */
            {
                // divTerminal always starts from bottom of the scroll
                if( currentChildNode.contains(divTerminal) ) {
                    divTerminal.scrollTop = divTerminal.scrollHeight;
                }
            }
        }
        else
        if (event.deltaY > 0 && currentChildIndex != this.children.length-1)
        {
            /* when wheel down evnets, contitions before swapping */
            {
            }

            $(currentChildNode).hide();
            $(currentChildNode = this.children[currentChildIndex+1]).show();
            currentChildNode.scrollTop = 0;

            /* when wheel down evnets, contitions after swapping */
            {
            }
        }
    });

    // textarea can be controlled by global key events
    document.addEventListener('keydown', function(event)
    {
        if (isRunning != 0)
            return;

        // ESC
        if (event.key === 'Escape') {
            if(!isEnabledTextarea) {
                setCurrentFilename('');
                taFileContent.value = '';

                focusSelector('');
            }

            disableTextarea();
        }
        else // Ctrl+S
        if (event.ctrlKey && event.key === 's') {
            if (!isEnabledTextarea)
                return;

            event.preventDefault();
            executePushFile(getCurrentFilename());
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

            const start = this.selectionStart;
            const end = this.selectionEnd;

            this.value = this.value.substring(0, start) + '\t' + this.value.substring(end);

            this.selectionStart = this.selectionEnd = start + 1;
        }
    });

    // can exit textarea's temporary disable state
    taFileContent.addEventListener("click", function()
    {
        if (isRunning != 0)
            return;

        if(!isEnabledTextarea && getCurrentFilename().length > 1)
            enableTextarea();
    });

    // textarea's buttons located at bottom right
    btnCreateFile.addEventListener("click", function()
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
    btnSaveFile.addEventListener("click", function(){ executePushFile(getCurrentFilename()); });
});
)=====";