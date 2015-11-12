#include <nan.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tcl.h>
#include <tk.h>

int InitProc( Tcl_Interp *interp )
{
    /* Basic init */
    if (Tcl_Init(interp) == TCL_ERROR)
        return TCL_ERROR;
    if (Tk_Init(interp) == TCL_ERROR)
        return TCL_ERROR;
    Tcl_StaticPackage(interp, "Tk", Tk_Init, Tk_SafeInit);
    /* Read your startup code */
    Tcl_Eval(interp, "grid [ttk::button .b -text \"Hello World\"]");
    return Tcl_DeleteInterp(interp);
} // end InitProc

void Method(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    Tk_Main(argc, argv, InitProc);
    //info.GetReturnValue().Set(Nan::New("world").ToLocalChecked());
}

void Init(v8::Local<v8::Object> exports) {
  exports->Set(Nan::New("hello").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(Method)->GetFunction());
}

NODE_MODULE(hello, Init)
