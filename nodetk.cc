#include <nan.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tcl.h>
#include <tk.h>
#include <sstream>
#include <iostream>
#include <unordered_map>


using namespace v8;


uv_idle_t idler;

std::unordered_map <std::string, Persistent<Function, CopyablePersistentTraits<Function>>> callbacks;

static Tcl_Interp *interp;

char *ppszArg[1]; 

void StartLoop(uv_idle_t* handle) {
    while (Tk_GetNumMainWindows() > 0) {
        Tcl_DoOneEvent(TCL_DONT_WAIT);
    }
    for(auto kv : callbacks) {
        callbacks[kv.first].Empty();
    } 
    printf("good bye\n");
    uv_idle_stop(handle);
}

int InitProc(Tcl_Interp *localInterp)
{
    /* Basic init */
    if (Tcl_Init(localInterp) == TCL_ERROR)
        return TCL_ERROR;
    if (Tk_Init(localInterp) == TCL_ERROR)
        return TCL_ERROR;
    Tcl_StaticPackage(localInterp, "Tk", Tk_Init, Tk_SafeInit);

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

    // Probably better to create a Tk_CreateEventHandler
    uv_idle_init(uv_default_loop(), &idler);
    uv_idle_start(&idler, StartLoop);
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
    info.GetReturnValue().Set(Nan::New(interp->result).ToLocalChecked());
}

void TclCreateCommand(const Nan::FunctionCallbackInfo<Value>& info) {
    if (info.Length() > 2) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }

    if (!info[0]->IsString()) {
        Nan::ThrowTypeError("Wrong arguments");
        return;
    }

    if (!info[1]->IsFunction()) {
        Nan::ThrowTypeError("Wrong arguments");
        return;
    }

    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    String::Utf8Value name(info[0]->ToString());

    Local<Function> localCb = Local<Function>::Cast(info[1]);
    Persistent<Function> pcb;
    callbacks[std::string(*name)] = pcb;
    callbacks[std::string(*name)].Reset(isolate, localCb);

    auto autocb = [] (ClientData Data, Tcl_Interp *localInterp, int argc, const char *argv[]) {
        Nan::HandleScope scope;

        const unsigned nargc = 0;
        Local<Value> nargv[nargc] = {  };
        Handle<Value> js_result;

        // call persistent function
        v8::Isolate* isolate = v8::Isolate::GetCurrent();

        v8::Local<v8::Function> localCb = v8::Local<v8::Function>::New(isolate, callbacks[argv[0]]);
        localCb->Call(Nan::GetCurrentContext()->Global(), nargc, nargv);

        return 0;
    };
    Tcl_CreateCommand(interp, std::string(*name).c_str(), autocb, (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL );
}

void TclGetVar(const Nan::FunctionCallbackInfo<Value>& info) {
    if (info.Length() > 1) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }

    if (!info[0]->IsString()) {
        Nan::ThrowTypeError("Wrong arguments");
        return;
    }

    String::Utf8Value param(info[0]->ToString());
    std::string var = std::string(*param); 
    std::stringstream strstream;
    std::string valueholder;
    strstream << Tcl_GetVar(interp, var.c_str(), 0);
    strstream >> valueholder;
    info.GetReturnValue().Set(Nan::New(valueholder).ToLocalChecked());
}


void Init(Local<Object> exports) {

    exports->Set(Nan::New("init").ToLocalChecked(),
            Nan::New<FunctionTemplate>(InitTclTk)->GetFunction());
    exports->Set(Nan::New("eval").ToLocalChecked(),
            Nan::New<FunctionTemplate>(TclEval)->GetFunction());
    exports->Set(Nan::New("createCommand").ToLocalChecked(),
            Nan::New<FunctionTemplate>(TclCreateCommand)->GetFunction());
    exports->Set(Nan::New("getVar").ToLocalChecked(),
            Nan::New<FunctionTemplate>(TclGetVar)->GetFunction());
}


NODE_MODULE(nodetk, Init)
