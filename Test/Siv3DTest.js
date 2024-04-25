// Test stub: throw JSException instead of abort.
Module["onAbort"] = function(e) { 
  if ((e !== "" && e !== "native code called abort()"))
  {
      Module["siv3dSetThrowJSException"](e);
  }

  if (e === -1) {
    // FIXME: -1 is thrown when unresolved function is called in emscripten 3.1.20.
    // We should migrate to emscripten 3.1.53, in which unresolved symbol report will be thrown.
    // 
    // abort(-1); // emscripten 3.1.20, not user friendly abort
    // abort("missing function: <some function name>"); // emscripten 3.1.53
    Module["siv3dSetThrowJSException"](e.toString());
  }
};
