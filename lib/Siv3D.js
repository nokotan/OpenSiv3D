mergeInto(LibraryManager.library, {
    glfwGetKeysSiv3D: function (windowid) {
        const window = GLFW.WindowFromId(windowid);
        if (!window) return 0;
        if (!window.keysBuffer) {
            window.keysBuffer = Module._malloc(349 /* GLFW_KEY_LAST + 1 */)
        }
        Module.HEAPU8.set(window.keys, window.keysBuffer);
        return window.keysBuffer;
    },
    glfwGetKeysSiv3D__sig: "ii",

    glfwGetMonitorInfo_Siv3D: function(handle, displayID, name, xpos, ypos, w, h, wx, wy, ww, wh) {
        setValue(displayID, 1, 'i32');
        setValue(name, allocate(intArrayFromString("HTML5 WebGL Canvas"), 'i8', ALLOC_NORMAL), 'i32');
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

    $videoElements: [],

    s3dOpenVideo: function(callback, callbackArg) {
        const constraint = {
            video: true,
            audio: false
        };

        navigator.mediaDevices.getUserMedia(constraint).then(
            stream => {
                const video = document.createElement("video");

                video.addEventListener('loadedmetadata', function() {
                    const idx = GL.getNewId(videoElements);

                    video.removeEventListener('loadedmetadata', arguments.callee);
                    videoElements[idx] = video;

                    if (callback) dynCall_vii(callback, callbackArg, idx);
                });

                video.srcObject = stream;                      
            }
        ).catch(_ => {
            if (callback) dynCall_vii(callback, callbackArg, 0);
        })
    },
    s3dOpenVideo__sig: "vii",
    s3dOpenVideo__deps: ["$videoElements"],

    s3dBindVideo: function(target, level, internalFormat, width, height, border, format, type, idx) {
        const video = videoElements[idx];
        GLctx.texImage2D(target, level, internalFormat, width, height, border, format, type, video);
    },
    s3dBindVideo__sig: "viiiiiiiii",
    s3dBindVideo__deps: ["$videoElements"],

    s3dPlayVideo: function(idx, callback, callbackArg) {
        const video = videoElements[idx];
        video.play().then(
            () => {
                if (callback) dynCall_vii(callback, callbackArg, 1);
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

    $activeTouches: [],
    
    $s3dOnTouchMove: function(e) {
        activeTouches = Array.from(e.touches);
    },
    s3dRegisterTouchCallback: function() {
        Module["canvas"].addEventListener("touchmove", s3dOnTouchMove);
    },
    s3dRegisterTouchCallback__deps: [ "$s3dOnTouchMove", "$activeTouches" ],
    s3dUnregisterTouchCallback: function() {
        Module["canvas"].removeEventListener("touchmove", s3dOnTouchMove);
    },
    s3dUnregisterTouchCallback__deps: [ "$s3dOnTouchMove" ],
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
});