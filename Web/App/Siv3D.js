mergeInto(LibraryManager.library, {
    //
    // GLFW Extensions
    //
    glfwGetKeysSiv3D: function (windowid) {
        const window = GLFW.WindowFromId(windowid);
        if (!window) return 0;
        if (!window.keysBuffer) {
            window.keysBuffer = Module["_malloc"](349 /* GLFW_KEY_LAST + 1 */)
        }
        Module["HEAPU8"].set(window.keys, window.keysBuffer);
        return window.keysBuffer;
    },
    glfwGetKeysSiv3D__sig: "ii",

    glfwGetMonitorInfo_Siv3D: function(handle, displayID, name, xpos, ypos, w, h, wx, wy, ww, wh) {
        setValue(displayID, 1, 'i32');
#if EMSCRIPTEN_VERSION === "2.0.4"
        setValue(name, allocate(intArrayFromString("HTML5 WebGL Canvas"), 'i8', ALLOC_NORMAL), 'i32');
#else
        setValue(name, allocate(intArrayFromString("HTML5 WebGL Canvas"), ALLOC_NORMAL), 'i32');
#endif
        setValue(xpos, 0, 'i32');
        setValue(ypos, 0, 'i32');
        setValue(w, window.screen.width, 'i32');
        setValue(h, window.screen.height, 'i32');
        setValue(wx, window.screenX, 'i32');
        setValue(wy, window.screenX, 'i32');
        setValue(ww, window.outerWidth, 'i32');
        setValue(wh, window.outerHeight, 'i32');
    },
    glfwGetMonitorInfo_Siv3D__sig: "viiiiiiiiiii",
    
    glfwGetMonitorRect_Siv3D: function(handle, xpos, ypos, w, h) {
        setValue(xpos, 0, 'i32');
        setValue(ypos, 0, 'i32');
        setValue(w, window.screen.width, 'i32');
        setValue(h, window.screen.height, 'i32');
    },
    glfwGetMonitorRect_Siv3D__sig: "viiiii",

    glGetBufferSubData: function(target, srcOffset, length, offset) {
        Module.ctx.getBufferSubData(target, srcOffset, Module.HEAPU8, offset, length)
    },
    glGetBufferSubData__sig: "viiii",

    //
    // MessageBox
    //
    s3dShowMessageBox: function(messagePtr, type) {
        const message = UTF8ToString(messagePtr);

        if (type === 0) {
            /* MessageBoxButtons.OK */
            window.alert(message);
            return 0; /* MessageBoxSelection.OK */
        } else if (type === 1) {
            /* MessageBoxButtons.OKCancel */
            return window.confirm(message) ? 0 /* MessageBoxSelection.OK */ : 1 /* MessageBoxSelection.Cancel */;
        }

        return 4; /* MessageBoxSelection.None */
    },
    s3dShowMessageBox__sig: "iii",

    //
    // WebCamera Support
    //
    $videoElements: [],

    s3dOpenVideo: function(width, height, callback, callbackArg) {
        const constraint = {
            video: { width, height },
            audio: false
        };

        navigator.mediaDevices.getUserMedia(constraint).then(
            stream => {
                const video = document.createElement("video");

                video.addEventListener('loadedmetadata', function onLoaded() {
                    const idx = GL.getNewId(videoElements);

                    video.removeEventListener('loadedmetadata', onLoaded);
                    videoElements[idx] = video;

                    if (callback) {{{ makeDynCall('vii', 'callback') }}}(callbackArg, idx);
                });

                video.srcObject = stream;                      
            }
        ).catch(_ => {
            if (callback) {{{ makeDynCall('vii', 'callback') }}}(callbackArg, 0);
        })
    },
    s3dOpenVideo__sig: "viiii",
    s3dOpenVideo__deps: ["$videoElements"],

    s3dBindVideo: function(target, level, internalFormat, width, height, border, format, type, idx) {
        const video = videoElements[idx];
        GLctx.texImage2D(target, level, internalFormat, width, height, border, format, type, video);
    },
    s3dBindVideo__sig: "viiiiiiiii",
    s3dBindVideo__deps: ["$videoElements"],

    s3dQueryCurrentTime: function(idx) {
        const video = videoElements[idx];
        return video.currentTime;
    },
    s3dQueryCurrentTime__sig: "di",
    s3dQueryCurrentTime__deps: ["$videoElements"],

    s3dPlayVideo: function(idx, callback, callbackArg) {
        const video = videoElements[idx];
        video.play().then(
            () => {
                if (callback) {{{ makeDynCall('vii', 'callback') }}}(callbackArg, 1);
            }
        ); 
    },
    s3dPlayVideo__sig: "vi",
    s3dPlayVideo__deps: ["$videoElements"],

    s3dStopVideo: function(idx) {
        const video = videoElements[idx];

        let stream = video.srcObject;
        let tracks = stream.getTracks();
      
        tracks.forEach(function(track) {
            track.stop();
        });
    },
    s3dStopVideo__sig: "vi",
    s3dStopVideo__deps: ["$videoElements"],

    //
    // MultiTouch Support
    //
    $activeTouches: [],
    
    $s3dOnTouchStart: function(e) {
        activeTouches = Array.from(e.touches);
        e.preventDefault();
    },
    $s3dOnTouchEnd: function(e) {
        activeTouches = Array.from(e.touches);
        e.preventDefault();
    },
    $s3dOnTouchMove: function(e) {
        activeTouches = Array.from(e.touches);
        e.preventDefault();
    },
    s3dRegisterTouchCallback: function() {
        Module["canvas"].addEventListener("touchstart", s3dOnTouchStart);
        Module["canvas"].addEventListener("touchmove", s3dOnTouchMove);
    },
    s3dRegisterTouchCallback__deps: [ "$s3dOnTouchMove", "$s3dOnTouchStart", "$activeTouches" ],
    s3dUnregisterTouchCallback: function() {
        Module["canvas"].removeEventListener("touchstart", s3dOnTouchStart);
        Module["canvas"].removeEventListener("touchmove", s3dOnTouchMove);
    },
    s3dUnregisterTouchCallback__deps: [ "$s3dOnTouchMove", "$s3dOnTouchStart" ],
    s3dGetPrimaryTouchPoint: function(pX, pY) {
        if (activeTouches.length > 0) {
            const touch = activeTouches[0];

            const rect = Module["canvas"].getBoundingClientRect();
            const cw = Module["canvas"].width;
            const ch = Module["canvas"].height;

            const scrollX = ((typeof window.scrollX !== 'undefined') ? window.scrollX : window.pageXOffset);
            const scrollY = ((typeof window.scrollY !== 'undefined') ? window.scrollY : window.pageYOffset);

            let adjustedX = touch.pageX - (scrollX + rect.left);
            let adjustedY = touch.pageY - (scrollY + rect.top);
  
            adjustedX = adjustedX * (cw / rect.width);
            adjustedY = adjustedY * (ch / rect.height);
            
            setValue(pX, adjustedX, 'double');
            setValue(pY, adjustedY, 'double');
            return 1;
        } else {
            return 0;
        }
    },
    s3dGetPrimaryTouchPoint__sig: "iii",

    //
    // AngelScript Support
    //
    s3dCallIndirect: function(funcPtr, funcTypes, retPtr, argsPtr) {
        let args = [];
        let funcTypeIndex = funcTypes;
        let argsPtrIndex = argsPtr;

        const retType = HEAPU8[funcTypeIndex++];

        while (true) {
            const funcType = HEAPU8[funcTypeIndex++];

            if (funcType === 0) break;

            switch (funcType) {
                case 105: // 'i':
                    args.push(HEAP32[argsPtrIndex >> 2]);
                    argsPtrIndex += 4;
                    break;
                case 102: // 'f':
                    args.push(HEAPF32[argsPtrIndex >> 2]);
                    argsPtrIndex += 4;
                    break;
                case 100: // 'd':
                    args.push(HEAPF64[argsPtrIndex >> 3]);
                    argsPtrIndex += 8;
                    break;
                default:
                    err("Unrecognized Function Type");
            }
        }

        const retValue = wasmTable.get(funcPtr).apply(null, args);

        switch (retType) {
            case 105: // 'i':
                HEAP32[retPtr >> 2] = retValue;
                break;
            case 102: // 'f':
                HEAPF32[retPtr >> 2] = retValue;
                break;
            case 100: // 'd':
                HEAPF64[retPtr >> 3] = retValue;
                break;
            case 118: // 'v':
                break;
            default:
                err("Unrecognized Function Type");
        }
    },
    s3dCallIndirect__sig: "viiii",
    s3dCallIndirectReturnInMemory: function(funcPtr, funcTypes, retPtr, argsPtr) {
        let args = [];
        let funcTypeIndex = funcTypes;
        let argsPtrIndex = argsPtr;
        
        const retType = HEAPU8[funcTypeIndex++];
        const retValPtr = HEAP32[argsPtrIndex >> 2];
        argsPtrIndex += 4;

        while (true) {
            const funcType = HEAPU8[funcTypeIndex++];

            if (funcType === 0) break;

            switch (funcType) {
                case 105: // 'i':
                    args.push(HEAP32[argsPtrIndex >> 2]);
                    argsPtrIndex += 4;
                    break;
                case 102: // 'f':
                    args.push(HEAPF32[argsPtrIndex >> 2]);
                    argsPtrIndex += 4;
                    break;
                case 100: // 'd':
                    args.push(HEAPF64[argsPtrIndex >> 3]);
                    argsPtrIndex += 8;
                    break;
                default:
                    err("Unrecognized Function Type");
            }
        }

        wasmTable.get(funcPtr).apply(null, args);
        HEAP32[retPtr >> 2] = retValPtr;
    },
    s3dCallIndirectReturnInMemory__sig: "viiii",

    //
    // User Action Emulation
    //
    $s3dHasUserActionTriggered: false,
    $s3dPendingUserActions: [],

    $s3dTriggerUserActionEmulation: function() {
        for (let action of s3dPendingUserActions) {
            action();
        }

        s3dPendingUserActions.splice(0);
    },
    $s3dTriggerUserActionEmulation__deps: [ "$s3dPendingUserActions" ],

    $s3dRegisterUserAction: function(func) {
        s3dPendingUserActions.push(func);
    },
    $s3dRegisterUserAction__deps: [ "$s3dPendingUserActions" ],

    $s3dUserActionHookCallBack: function() {
        if (!s3dHasUserActionTriggered) {
            setTimeout(s3dTriggerUserActionEmulation, 30);
            s3dHasUserActionTriggered = true;
        }
    },
    $s3dUserActionHookCallBack__deps: [ "$s3dHasUserActionTriggered", "$s3dTriggerUserActionEmulation" ],

    s3dStartUserActionHook: function() {
        Module["canvas"].addEventListener('touchstart', s3dUserActionHookCallBack);
        Module["canvas"].addEventListener('mousedown', s3dUserActionHookCallBack);
        window.addEventListener('keydown', s3dUserActionHookCallBack);
    },
    s3dStartUserActionHook__sig: "v",
    s3dStartUserActionHook__deps: [ "$s3dUserActionHookCallBack", "$s3dHasUserActionTriggered" ],

    s3dResetUserActionFlag: function() {
        s3dHasUserActionTriggered = false;
    },
    s3dResetUserActionFlag__sig: "v",
    s3dResetUserActionFlag__deps: [ "$s3dHasUserActionTriggered" ],

    s3dStopUserActionHook: function() {
        Module["canvas"].removeEventListener('touchstart', s3dUserActionHookCallBack);
        Module["canvas"].removeEventListener('mousedown', s3dUserActionHookCallBack);
        window.removeEventListener('keydown', s3dUserActionHookCallBack);
    },
    s3dStopUserActionHook__sig: "v",
    s3dStopUserActionHook__deps: [ "$s3dUserActionHookCallBack" ],

    //
    // Dialog Support
    //
    $s3dInputElement: null,
    $s3dDialogFileReader: null,
    $s3dDownloadLink: null,

    s3dInitDialog: function() {
        s3dInputElement = document.createElement("input");
        s3dInputElement.type = "file";

        s3dDialogFileReader = new FileReader();

        s3dSaveFileBuffer = new Uint8Array(16*1024 /* 16KB */)
        s3dDownloadLink = document.createElement("a");

        TTY.register(FS.makedev(20, 0), { put_char: s3dWriteSaveFileBuffer, flush: s3dFlushSaveFileBuffer });
        FS.mkdev('/dev/save', FS.makedev(20, 0));
    },
    s3dInitDialog__sig: "v",
    s3dInitDialog__deps: [ "$s3dInputElement", "$s3dDialogFileReader", "$s3dWriteSaveFileBuffer", "$s3dFlushSaveFileBuffer", "$s3dSaveFileBuffer", "$s3dDownloadLink", "$TTY", "$FS" ],

    s3dOpenDialog: function(filterStr, callback, futurePtr) {
        s3dInputElement.accept = UTF8ToString(filterStr);
        s3dInputElement.oninput = function(e) {
            const files = e.target.files;

            if (files.length < 1) {
                {{{ makeDynCall('vii', 'callback') }}}(0, futurePtr);
                return;
            }

            const file = files[0];
            const filePath = `/tmp/${file.name}`;

            s3dDialogFileReader.addEventListener("load", function onLoaded() {
                FS.writeFile(filePath, new Uint8Array(s3dDialogFileReader.result));
#if EMSCRIPTEN_VERSION === "2.0.4"
                const namePtr = allocate(intArrayFromString(filePath), 'i8', ALLOC_NORMAL);
#else
                const namePtr = allocate(intArrayFromString(filePath), ALLOC_NORMAL);
#endif
                {{{ makeDynCall('vii', 'callback') }}}(namePtr, futurePtr);

                s3dDialogFileReader.removeEventListener("load", onLoaded);
            });

            s3dDialogFileReader.readAsArrayBuffer(file);         
        };

        s3dRegisterUserAction(function() {
            s3dInputElement.click();
        });
    },
    s3dOpenDialog__sig: "vii",
    s3dOpenDialog__deps: [ "$s3dInputElement", "$s3dDialogFileReader", "$s3dRegisterUserAction", "$FS" ],

    $s3dSaveFileBuffer: null, 
    $s3dSaveFileBufferWritePos: 0,
    $s3dDefaultSaveFileName: null,

    $s3dWriteSaveFileBuffer: function(tty, chr) {       
        if (s3dSaveFileBufferWritePos >= s3dSaveFileBuffer.length) {
            const newBuffer = new Uint8Array(s3dSaveFileBuffer.length * 2);
            newBuffer.set(s3dSaveFileBuffer);
            s3dSaveFileBuffer = newBuffer;
        }

        s3dSaveFileBuffer[s3dSaveFileBufferWritePos] = chr;
        s3dSaveFileBufferWritePos++;
    },
    $s3dWriteSaveFileBuffer__deps: [ "$s3dSaveFileBuffer", "$s3dSaveFileBufferWritePos" ], 
    $s3dFlushSaveFileBuffer: function(tty) {
        if (s3dSaveFileBufferWritePos == 0) {
            return;
        }

        const data = s3dSaveFileBuffer.subarray(0, s3dSaveFileBufferWritePos);
        const blob = new Blob([ data ], { type: "application/octet-stream" });

        s3dDownloadLink.href = URL.createObjectURL(blob);
        s3dDownloadLink.download = s3dDefaultSaveFileName;

        s3dRegisterUserAction(function() {
            s3dDownloadLink.click();         
        });

        s3dSaveFileBufferWritePos = 0;
    },
    $s3dWriteSaveFileBuffer__deps: [ "$s3dSaveFileBuffer", "$s3dSaveFileBufferWritePos", "$s3dRegisterUserAction", "$s3dDefaultSaveFileName", "$s3dDownloadLink" ], 

    s3dSaveDialog: function(str) {
        s3dDefaultSaveFileName = UTF8ToString(str);
        s3dSaveFileBufferWritePos = 0;
    },
    s3dSaveDialog__sig: "v",
    s3dSaveDialog__deps: [ "$s3dSaveFileBufferWritePos", "$s3dDefaultSaveFileName" ],

    //
    // Audio Support
    //
    s3dDecodeAudioFromFile: function(filePath, callbackPtr, arg) {
        const path = UTF8ToString(filePath, 1024);
        const fileBytes = FS.readFile(path);

        const onSuccess = function(decoded) {
            const leftDataBuffer = Module["_malloc"](decoded.length * 4);
            HEAPF32.set(decoded.getChannelData(0), leftDataBuffer>>2);

            let rightDataBuffer;
            
            if (decoded.numberOfChannels >= 2) {
                rightDataBuffer = Module["_malloc"](decoded.length * 4);
                HEAPF32.set(decoded.getChannelData(1), rightDataBuffer>>2);
            } else {
                rightDataBuffer = leftDataBuffer;
            }

            HEAP32[(arg>>2)+0] = leftDataBuffer;
            HEAP32[(arg>>2)+1] = rightDataBuffer;
            HEAPU32[(arg>>2)+2] = decoded.sampleRate;
            HEAPU32[(arg>>2)+3] = decoded.length;

            {{{ makeDynCall('vi', 'callbackPtr') }}}(arg);
        };

        const onFailure = function() {
            HEAP32[(arg>>2)+0] = 0;
            HEAP32[(arg>>2)+1] = 0;
            HEAPU32[(arg>>2)+2] = 0;
            HEAPU32[(arg>>2)+3] = 0;

            {{{ makeDynCall('vi', 'callbackPtr') }}}(arg);
        }

        AL.currentCtx.audioCtx.decodeAudioData(fileBytes.buffer, onSuccess, onFailure);   
    },
    s3dDecodeAudioFromFile__sig: "vii",
    s3dDecodeAudioFromFile__deps: [ "$AL", "$FS" ],

    //
    // DragDrop Support
    //
    s3dRegisterDragEnter: function(ptr) {
        Module["canvas"].ondragenter = function (e) {
            e.preventDefault();

            const types = e.dataTransfer.types;

            if (types.length > 0) {
                {{{ makeDynCall('vi', 'ptr') }}}(types[0] === 'Files' ? 1 : 0);
            }        
        };
    },
    s3dRegisterDragEnter__sig: "vi",

    s3dRegisterDragUpdate: function(ptr) {
        Module["canvas"].ondragover = function (e) {
            e.preventDefault();
            {{{ makeDynCall('v', 'ptr') }}}();
        };
    },
    s3dRegisterDragUpdate__sig: "vi",

    s3dRegisterDragExit: function(ptr) {
        Module["canvas"].ondragexit = function (e) {
            e.preventDefault();
            {{{ makeDynCall('v', 'ptr') }}}();
        };
    },
    s3dRegisterDragExit__sig: "vi",

    $s3dDragDropFileReader: null,
    s3dRegisterDragDrop: function(ptr) {
        Module["canvas"].ondrop = function (e) {
            e.preventDefault();

            const items = e.dataTransfer.items;

            if (items.length == 0) {
                return;
            }

            if (items[0].kind === 'text') {
                items[0].getAsString(function(str) {
#if EMSCRIPTEN_VERSION === "2.0.4"
                    const strPtr = allocate(intArrayFromString(str), 'i8', ALLOC_NORMAL);
#else
                    const strPtr = allocate(intArrayFromString(str), ALLOC_NORMAL);
#endif
                    {{{ makeDynCall('vi', 'ptr') }}}(strPtr);
                    Module["_free"](strPtr);
                })            
            } else if (items[0].kind === 'file') {
                const file = items[0].getAsFile();

                if (!s3dDragDropFileReader) {
                    s3dDragDropFileReader = new FileReader();
                }

                const filePath = `/tmp/${file.name}`;

                s3dDragDropFileReader.addEventListener("load", function onLoaded() {
                    FS.writeFile(filePath, new Uint8Array(s3dDragDropFileReader.result));
                
#if EMSCRIPTEN_VERSION === "2.0.22"
                    const namePtr = allocate(intArrayFromString(filePath), 'i8', ALLOC_NORMAL);
#else
                    const namePtr = allocate(intArrayFromString(filePath), ALLOC_NORMAL);
#endif

                    {{{ makeDynCall('vi', 'ptr') }}}(namePtr);

                    s3dDragDropFileReader.removeEventListener("load", onLoaded);
                });

                s3dDragDropFileReader.readAsArrayBuffer(file);              
            }
        };
    },
    s3dRegisterDragDrop__sig: "vi",
    s3dRegisterDragDrop__deps: [ "$s3dDragDropFileReader", "$FS" ],

    //
    // Cursor
    //
    s3dSetCursorStyle: function(style) {
        const styleText = UTF8ToString(style);
        Module["canvas"].style.cursor = styleText;
    },
    s3dSetCursorStyle__sig: "vi",

    //
    // Clipboard
    //
    s3dSetClipboardText: function(ctext) {
        const text = UTF8ToString(ctext);
        
        s3dRegisterUserAction(function () {
            navigator.clipboard.writeText(text);
        });
    },
    s3dSetClipboardText__sig: "vi",
    s3dSetClipboardText__deps: [ "$s3dRegisterUserAction" ],

    s3dGetClipboardText: function(callback, promise) {
        s3dRegisterUserAction(function () {
            navigator.clipboard.readText()
            .then(str => {
                const strPtr = allocate(intArrayFromString(str), 'i8', ALLOC_NORMAL);       
                {{{ makeDynCall('vii', 'callback') }}}(strPtr, promise);
                Module["_free"](strPtr);
            })
            .catch(e => {
                {{{ makeDynCall('vii', 'callback') }}}(0, promise);
            })
        });
        
    },
    s3dGetClipboardText__sig: "vii",
    s3dGetClipboardText__deps: [ "$s3dRegisterUserAction" ],

    //
    // TextInput
    //
    $s3dTextInputElement: null,

    s3dInitTextInput: function() {
        const textInput = document.createElement("input");
        textInput.type = "text";
        textInput.style.position = "absolute";
        textInput.style.zIndex = -2;
        textInput.autocomplete = false;

        const maskDiv = document.createElement("div");
        maskDiv.style.background = "white";
        maskDiv.style.position = "absolute";
        maskDiv.style.width = "100%";
        maskDiv.style.height = "100%";
        maskDiv.style.zIndex = -1;

        /**
         * @type { HTMLCanvasElement }
         */
        const canvas = Module["canvas"];

        canvas.parentNode.prepend(textInput);
        canvas.parentNode.prepend(maskDiv);

        s3dTextInputElement = textInput;
    },
    s3dInitTextInput__sig: "v",
    s3dInitTextInput__deps: [ "$s3dTextInputElement" ],

    s3dRegisterTextInputCallback: function(callback) {
        s3dTextInputElement.addEventListener('input', function (e) {
            if (e.inputType == "insertText") {
                if (e.data) {
                    for (let i = 0; i < e.data.length; i++) {
                        const codePoint = e.data.charCodeAt(i);
                        {{{ makeDynCall('vi', 'callback') }}}(codePoint);
                    }
                }
            }    
        });
        s3dTextInputElement.addEventListener('compositionend', function (e) {
            for (let i = 0; i < e.data.length; i++) {
                const codePoint = e.data.charCodeAt(i);
                {{{ makeDynCall('vi', 'callback') }}}(codePoint);
            }
        });
    },
    s3dRegisterTextInputCallback__sig: "vi",
    s3dRegisterTextInputCallback__deps: [ "$s3dTextInputElement" ],

    s3dRegisterTextInputMarkedCallback: function(callback) {
        s3dTextInputElement.addEventListener('compositionupdate', function (e) {
            const strPtr = allocate(intArrayFromString(e.data), 'i8', ALLOC_NORMAL);
            {{{ makeDynCall('vi', 'callback') }}}(strPtr);
            Module["_free"](strPtr);
        })
        s3dTextInputElement.addEventListener('compositionend', function (e) {
            {{{ makeDynCall('vi', 'callback') }}}(0);
        });
    },
    s3dRegisterTextInputMarkedCallback__sig: "vi",
    s3dRegisterTextInputMarkedCallback__deps: [ "$s3dTextInputElement" ],

    s3dRequestTextInputFocus: function(isFocusRequired) {
        const isFocusRequiredBool = isFocusRequired != 0;

        if (isFocusRequiredBool) {
            s3dRegisterUserAction(function () {
                s3dTextInputElement.value = ""
                s3dTextInputElement.focus();
            });
        } else {
            s3dRegisterUserAction(function () {
                s3dTextInputElement.blur();
            });
        }
    },
    s3dRequestTextInputFocus__sig: "vi",
    s3dRequestTextInputFocus__deps: [ "$s3dRegisterUserAction", "$s3dTextInputElement" ],

    //
    // Notification
    //
    $s3dNotifications: [],

    s3dRequestNotificationPermission: function(callback, callbackArg) {
        if (Notification.permission === "granted") {
            {{{ makeDynCall('vii', 'callback') }}}(1 /* NotificationPermission.Granted */, callbackArg);
        } else {
            Notification.requestPermission().then(function(v) {
                if (v === "granted") {
                    {{{ makeDynCall('vii', 'callback') }}}(1 /* NotificationPermission.Granted */, callbackArg);
                } else {
                    {{{ makeDynCall('vii', 'callback') }}}(2 /* NotificationPermission.Denied */, callbackArg);
                }
            });
        }
    },
    s3dRequestNotificationPermission__sig: "vii",

    s3dCreateNotification: function(title, body, actionsNum, actionTexts, callback, callbackArg) {
        if (!window.Notification && Notification.permission !== "granted") {
            {{{ makeDynCall('vii', 'callback') }}}(0, callbackArg);
            return 0;
        }

        const idx = GL.getNewId(s3dNotifications);

        const titleText = UTF8ToString(title);
        const bodyText = UTF8ToString(body);
        let actions = [];

        for (let i = 0; i < actionsNum; i++) {
            const textPtr = getValue(actionTexts + i * 4, "i32");
            const actionText = UTF8ToString(textPtr);

            actions.push({ title: actionText, action: actionText });
        }

        s3dRegisterUserAction(function () {
            s3dNotifications[idx] = new Notification(titleText, { body: bodyText, actions: actions })
            {{{ makeDynCall('vii', 'callback') }}}(idx, callbackArg);
        }); 

        return idx;
    },
    s3dCreateNotification__sig: "iiiiiii",
    s3dCreateNotification__deps: [ "$s3dRegisterUserAction", "$s3dNotifications" ],

    s3dRegisterNotificationCallback: function(id, callback, callbackArg) {
        const notificattion = s3dNotifications[id];

        notificattion.onclick = function() {
            {{{ makeDynCall('viii', 'callback') }}}(id, 1 /* ToastNotificationState.Activated */, callbackArg);
        }
        notificattion.onshow = function() {
            {{{ makeDynCall('viii', 'callback') }}}(id, 2 /* ToastNotificationState.Shown */, callbackArg);
        }
        notificattion.onclose = function() {
            {{{ makeDynCall('viii', 'callback') }}}(id, 5 /* ToastNotificationState.TimedOut */, callbackArg);
        }
        notificattion.onerror = function() {
            {{{ makeDynCall('viii', 'callback') }}}(id, 6 /* ToastNotificationState.Error */, callbackArg);
        }
    },
    s3dRegisterNotificationCallback__sig: "viii",
    s3dRegisterNotificationCallback__deps: [ "$s3dNotifications" ],

    s3dCloseNotification: function(id) {
        const notificattion = s3dNotifications[id];
        notificattion.close();

        delete s3dNotifications[id];
    },
    s3dCloseNotification__sig: "vi",
    s3dCloseNotification__deps: [ "$s3dNotifications" ],

    s3dQueryNotificationPermission: function() {
        const status = {
            "default": 0,
            "granted": 1,
            "denied": 2
        };
        return status[Notification.permission];
    },
    s3dQueryNotificationPermission__sig: "iv",

    //
    // TextToSpeech
    //
    s3dEnumerateAvailableTextToSpeechLanguages: function(returnPtr) {
        const LanguageNameToLanguageCodeList = {
            "ar-SA": 1025,
            "zh-CN": 2052,
            "zh-HK": 3076,
            "zh-TW": 1028,
            "en-AU": 3081,
            "en-GB": 2057,
            "en-US": 1033,
            "fr-FR": 1036,
            "de-DE": 1031,
            "hi-IN": 1081,
            "it-IT": 1040,
            "ja-JP": 1041,
            "ko-KR": 1042,
            "pt-BR": 1046,
            "ru-RU": 1049,
            "es-ES": 1034
        };
        
        const voices = window.speechSynthesis.getVoices();
        let listBufferPtr = Module["_malloc"](voices.length * 4 * 2);

        setValue(returnPtr, voices.length, "i32");
        setValue(returnPtr + 4, listBufferPtr, "i32");

        for(let i = 0; i < voices.length; i++) {
            const languageCode = LanguageNameToLanguageCodeList[voices[i].lang];
             
            setValue(listBufferPtr + 0, languageCode, "i32");
            setValue(listBufferPtr + 4, voices[i].default, "i32");

            listBufferPtr += 8;
        }
    },
    s3dEnumerateAvailableTextToSpeechLanguages__sig: "vi",

    s3dStartTextToSpeechLanguages: function(textPtr, rate, volume, languageCode) {
        const LanguageCodeToLanguageNameList = {
            1025: "ar-SA",
            2052: "zh-CN",
            3076: "zh-HK",
            1028: "zh-TW",
            3081: "en-AU",
            2057: "en-GB",
            1033: "en-US",
            1036: "fr-FR",
            1031: "de-DE",
            1081: "hi-IN",
            1040: "it-IT",
            1041: "ja-JP",
            1042: "ko-KR",
            1046: "pt-BR",
            1049: "ru-RU",
            1034: "es-ES"
        };
        const text = UTF8ToString(textPtr);

        const speechUtter = new SpeechSynthesisUtterance(text);

        speechUtter.lang = LanguageCodeToLanguageNameList[languageCode];
        speechUtter.rate = rate;
        speechUtter.volume = volume;

        window.speechSynthesis.speak(speechUtter);
    },
    s3dStartTextToSpeechLanguages__sig: "viiii",

    s3dIsSpeakingTextToSpeechLanguages: function() {
        return window.speechSynthesis.speaking;
    },
    s3dIsSpeakingTextToSpeechLanguages__sig: "iv",

    s3dPauseTextToSpeechLanguages: function() {
        window.speechSynthesis.pause();
    },
    s3dPauseTextToSpeechLanguages__sig: "v",

    s3dResumeTextToSpeechLanguages: function() {
        window.speechSynthesis.resume();
    },
    s3dResumeTextToSpeechLanguages__sig: "v",

    //
    // Misc
    //
    s3dLaunchBrowser: function(url) {
        const urlString = UTF32ToString(url);
        
        s3dRegisterUserAction(function () {
            window.open(urlString, '_blank')
        });
    },
    s3dLaunchBrowser__sig: "vi",
    s3dLaunchBrowser__deps: [ "$s3dRegisterUserAction" ],
});