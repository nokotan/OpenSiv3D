(function() 
{
    const onAbort = Module["onAbort"] || ((e) => {
        window.alert(e);
    });

    Module["onAbort"] = function(e) 
    {
        if (e instanceof WebAssembly.CompileError)
        {
            const additionalMessage = 
                "The application failed to launch with WebAssembly compilation failure. \
Maybe some webassembly features are required to execute this application. \
Please refer https://webassembly.org/roadmap/ to check which webassembly features are supported in your browser. \
\
Original Error: " + e;
            
            onAbort(additionalMessage);
        }
        else if (e instanceof ProgressEvent)
        {
            onAbort("Browser seems to blocked fetch required assets. \
The application has been launched from explorer, which is not supported. \
Please launch emrun or your favorite HTTP server and access this application through it.");
        }
        else if (e instanceof ReferenceError && e.message.includes("WebAssembly"))
        {
            onAbort("The application cannot be launched with this browser. \
The application requires that this browser supports WebAssembly, which seems to be not available in this browser.\
Please use another browser that supports WebAssembly.")
        }
        else
        {
            onAbort(e);
        }
        // Test stub: throw JSException instead of abort.
        // else if (typeof e === "string" && e !== "" && e !== "native code called abort()")
        // {
        //     Module["siv3dSetThrowJSException"](e);
        // }
    };
})();
