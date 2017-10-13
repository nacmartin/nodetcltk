//var nodetk = require('bindings')('nodetk');
var nodetk = require('./index.js');

nodetk.init();
nodetk.eval("wm title . \"Feet to Meters\";");
nodetk.eval("grid [ttk::frame .c -padding \"3 3 12 12\"] -column 0 -row 0 -sticky nwes;");
nodetk.eval("grid columnconfigure . 0 -weight 1;");
nodetk.eval("grid rowconfigure . 0 -weight 1;");
nodetk.eval("grid [ttk::entry .c.feet -width 7 -textvariable feet] -column 2 -row 1 -sticky we;");
nodetk.eval("grid [ttk::label .c.meters -textvariable meters] -column 2 -row 2 -sticky we;");
nodetk.eval("grid [ttk::button .c.calc -text \"Calculate\" -command multi] -column 3 -row 3 -sticky w;");
nodetk.eval("grid [ttk::label .c.flbl -text \"feet\"] -column 3 -row 1 -sticky w");
nodetk.eval("grid [ttk::label .c.islbl -text \"is equivalent to\"] -column 1 -row 2 -sticky e;");
nodetk.eval("grid [ttk::label .c.mlbl -text \"meters\"] -column 3 -row 2 -sticky w;");

nodetk.eval("winfo children .c").split(' ').map((child) => {
    nodetk.eval(`grid configure ${child} -padx 5 -pady 5`);
});
nodetk.eval("focus .c.feet;");
nodetk.eval("bind . <Return> {multi};");

var toMeters = function () {
    return nodetk.eval('set ::meters ' + nodetk.getVar('::feet') * 0.3048);
}

nodetk.createCommand('multi', toMeters);

console.log("can execute code after tk init");

