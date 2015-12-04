#include <nan.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tcl.h>
#include <tk.h>
#include <sstream>
#include <iostream>

using namespace v8;


uv_idle_t idler;
Persistent<Function, CopyablePersistentTraits<Function>> cb;

static Tcl_Interp *interp;

char *ppszArg[1]; 

void wait_for_a_while(uv_idle_t* handle) {
    while (Tk_GetNumMainWindows() > 0) {
        Tcl_DoOneEvent(TCL_DONT_WAIT);
    }
    printf("good bye\n");
    cb.Empty();
    uv_idle_stop(handle);
}

int Multi( ClientData Data, Tcl_Interp *localInterp, int argc, const char *argv[] ) {

    std::string feet;
    std::string meters;
    std::stringstream strstream;
    strstream << Tcl_GetVar(interp, "::feet", 0);
    strstream >> feet;

    Nan::HandleScope scope;

    const unsigned nargc = 1;
    Local<Value> nargv[nargc] = { Nan::New(feet).ToLocalChecked() };
    Handle<Value> js_result;

    // call persistent function
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::Local<v8::Function> localCb = v8::Local<v8::Function>::New(isolate, cb);
    js_result = localCb->Call(Nan::GetCurrentContext()->Global(), nargc, nargv);

    // cast result
    String::Utf8Value bar(js_result->ToString());

    std::string met(*bar);

    meters = "set ::meters "+ met;
    Tcl_Eval(localInterp, meters.c_str());

    return 0;
}

int InitProc( Tcl_Interp *localInterp )
{
    /* Basic init */
    if (Tcl_Init(localInterp) == TCL_ERROR)
        return TCL_ERROR;
    if (Tk_Init(localInterp) == TCL_ERROR)
        return TCL_ERROR;
    Tcl_StaticPackage(localInterp, "Tk", Tk_Init, Tk_SafeInit);
    /* Make your commands here */
    //Tcl_CreateCommand(interp, "set_parser",           SET_Parser, NULL, NULL);
    /* Read your startup code */

    Tcl_Eval(localInterp, "");
    return TCL_OK;
}

void InitTclTk(const Nan::FunctionCallbackInfo<Value>& info) {
    if (info.Length() > 0) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }

    interp = Tcl_CreateInterp();

    InitProc(interp);

    uv_idle_init(uv_default_loop(), &idler);
    uv_idle_start(&idler, wait_for_a_while);
}

void TclEval(const Nan::FunctionCallbackInfo<Value>& info) {
    if (info.Length() > 1) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }

    if (!info[0]->IsString()) {
        Nan::ThrowTypeError("Wrong arguments");
        return;
    }

    String::Utf8Value param1(info[0]->ToString());
    std::string command = std::string(*param1); 
    Tcl_Eval(interp, command.c_str());
}

void TclCreateCommand(const Nan::FunctionCallbackInfo<Value>& info) {
    if (info.Length() > 1) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }

    if (!info[0]->IsFunction()) {
        Nan::ThrowTypeError("Wrong arguments");
        return;
    }

    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    Local<Function> localCb = Local<Function>::Cast(info[0]);
    cb.Reset(isolate, localCb);

    Tcl_CreateCommand(interp, "multi", Multi, (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL );

}

void Init(Local<Object> exports) {

    exports->Set(Nan::New("init").ToLocalChecked(),
            Nan::New<FunctionTemplate>(InitTclTk)->GetFunction());
    exports->Set(Nan::New("eval").ToLocalChecked(),
            Nan::New<FunctionTemplate>(TclEval)->GetFunction());
    exports->Set(Nan::New("createCommand").ToLocalChecked(),
            Nan::New<FunctionTemplate>(TclCreateCommand)->GetFunction());
}

NODE_MODULE(nodetk, Init)
