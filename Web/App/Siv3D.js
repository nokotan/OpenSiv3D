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
    s3dGetPrimaryTouchPoint__sig: "iii"
});