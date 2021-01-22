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
        setValue(name, allocate(intArrayFromString("HTML5 WebGL Canvas"), ALLOC_NORMAL), 'i32');
        setValue(xpos, 0, 'i32');
        setValue(ypos, 0, 'i32');
        setValue(w, 0, 'i32');
        setValue(h, 0, 'i32');
        setValue(wx, 0, 'i32');
        setValue(wy, 0, 'i32');
        setValue(ww, 0, 'i32');
        setValue(wh, 0, 'i32');
    },
    glfwGetMonitorInfo_Siv3D__sig: "viiiiiiiiiii",
    
    glfwGetMonitorRect_Siv3D: function(handle, xpos, ypos, w, h) {
        setValue(xpos, 0, 'i32');
        setValue(ypos, 0, 'i32');
        setValue(w, 0, 'i32');
        setValue(h, 0, 'i32');
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
    // Dialog Support
    //
    $s3dInputElement: null,
    $s3dDialogFileReader: null,
    $s3dIsPindingDialog: false,
    $s3dMayOpenDialog: false,

    $s3dInitDialog: function() {
        s3dInputElement = document.createElement("input");
        s3dInputElement.type = "file";

        s3dDialogFileReader = new FileReader();

        function OpenDialogHelper() {
            if (!s3dMayOpenDialog) {
                setTimeout(function() {
                    if (s3dIsPindingDialog) {
                        s3dInputElement.click();
                        s3dIsPindingDialog = false;
                    }
                }, 50);
                s3dMayOpenDialog = true;
            }     
        }

        Module["canvas"].addEventListener('touchstart', OpenDialogHelper);
        Module["canvas"].addEventListener('mousedown', OpenDialogHelper);
        Module["canvas"].addEventListener('keydown', OpenDialogHelper);

        Module['postMainLoop'] = function() {
            s3dMayOpenDialog = false;
        }
    },
    $s3dInitDialog__deps: [ "$s3dMayOpenDialog" ],

    s3dOpenDialog: function(callback, futurePtr) {
        if (!s3dInputElement) {
            s3dInitDialog();
            return;
        }

        s3dInputElement.oninput = function onChange(e) {
            const files = e.target.files;

            if (files.length < 1) {
                {{{ makeDynCall('vii', 'callback') }}}(futurePtr, 0);
                return;
            }

            const file = files[0];
            const filePath = `/tmp/${file.name}`;

            s3dDialogFileReader.addEventListener("load", function onLoaded() {
                FS.writeFile(filePath, new Uint8Array(s3dDialogFileReader.result));

                const namePtr = allocate(intArrayFromString(filePath), 'i8', ALLOC_NORMAL);
                {{{ makeDynCall('vii', 'callback') }}}(futurePtr, namePtr);

                s3dDialogFileReader.removeEventListener("load", onLoaded);
            });

            s3dDialogFileReader.readAsArrayBuffer(file);         
        };

        s3dIsPindingDialog = true;
    },
    s3dOpenDialog__sig: "vii",
    s3dOpenDialog__deps: [ "$s3dInputElement", "$s3dDialogFileReader", "$s3dIsPindingDialog", "$s3dInitDialog", "$FS" ],

    //
    // Audio Support
    //
    s3dDecodeAudioFromFile: function(filePath, callbackPtr, arg) {
        const path = UTF8ToString(filePath, 1024);
        const fileBytes = FS.readFile(path);

        const callBack = function(decoded) {
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

        AL.currentCtx.audioCtx.decodeAudioData(fileBytes.buffer, callBack);   
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
                    const strPtr = allocate(intArrayFromString(str), 'i8', ALLOC_NORMAL);
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

                    const namePtr = allocate(intArrayFromString(filePath), 'i8', ALLOC_NORMAL);
                    {{{ makeDynCall('vi', 'ptr') }}}(namePtr);

                    s3dDragDropFileReader.removeEventListener("load", onLoaded);
                });

                s3dDragDropFileReader.readAsArrayBuffer(file);              
            }
        };
    },
    s3dRegisterDragDrop__sig: "vi",
    s3dRegisterDragDrop__deps: [ "$s3dDragDropFileReader" ],

});