var nodetk = require('./index.js');

var countries = [
    {code: 'ar', name: 'Argentina', population: '41000000'},
    {code: 'au', name: 'Australia', population: '21179211'},
    {code: 'be', name: 'Belgium', population: '10584534'},
    {code: 'br', name: 'Brazil', population: '185971537'},
    {code: 'ca', name: 'Canada', population: '33148682'},
    {code: 'cn', name: 'China', population: '1323128240'},
    {code: 'dk', name: 'Denmark', population: '5457415'},
    {code: 'fi', name: 'Finland', population: '5302000'},
    {code: 'fr', name: 'France', population: '64102140'},
    {code: 'gr', name: 'Greece', population: '11147000'},
    {code: 'in', name: 'India', population: '1131043000'},
    {code: 'it', name: 'Italy', population: '59206382'},
    {code: 'jp', name: 'Japan', population: '127718000'},
    {code: 'mx', name: 'Mexico', population: '106535000'},
    {code: 'nl', name: 'Netherlands', population: '16402414'},
    {code: 'no', name: 'Norway', population: '4738085'},
    {code: 'es', name: 'Spain', population: '45116894'},
    {code: 'se', name: 'Sweden', population: '9174082'},
    {code: 'ch', name: 'Switzerland', population: '7508700'}
];


var countryNames = countries.map((country) => { return country.name; });
var countryCodes = countries.map((country) => { return country.code; });

nodetk.init();
nodetk.eval(`set countrycodes [list ${countryCodes.join(' ')}];`);
nodetk.eval(`set countrynames [list ${countryNames.join(' ')}];`);

nodetk.eval("array set gifts [list card \"Greeting card\" flowers \"Flowers\" nastygram \"Nastygram\"];");

nodetk.eval("grid [ttk::frame .c -padding \"5 5 12 0\"] -column 0 -row 0 -sticky nwes;");
nodetk.eval("grid columnconfigure . 0 -weight 1; grid rowconfigure . 0 -weight 1;");

nodetk.eval("set ::lbox [tk::listbox .c.countries -listvariable countrynames -height 5];");
nodetk.eval("ttk::label .c.lbl -text \"Send to country's leader:\";");
nodetk.eval("ttk::radiobutton .c.g1 -text $gifts(card) -variable gift -value card;");
nodetk.eval("ttk::radiobutton .c.g2 -text $gifts(flowers) -variable gift -value flowers;");
nodetk.eval("ttk::radiobutton .c.g3 -text $gifts(nastygram) -variable gift -value nastygram;");
nodetk.eval("ttk::button .c.send -text \"Send Gift\" -command {sendGift} -default active;");
nodetk.eval("ttk::label .c.sentlbl -textvariable sentmsg -anchor center;");
nodetk.eval("ttk::label .c.status -textvariable statusmsg -anchor w;");
nodetk.eval("grid .c.countries -column 0 -row 0 -rowspan 6 -sticky nsew;");
nodetk.eval("grid .c.lbl       -column 1 -row 0 -padx 10 -pady 5;");
nodetk.eval("grid .c.g1        -column 1 -row 1 -sticky w -padx 20;");
nodetk.eval("grid .c.g2        -column 1 -row 2 -sticky w -padx 20;");
nodetk.eval("grid .c.g3        -column 1 -row 3 -sticky w -padx 20;");
nodetk.eval("grid .c.send      -column 2 -row 4 -sticky e;");
nodetk.eval("grid .c.sentlbl   -column 1 -row 5 -columnspan 2 -sticky n -pady 5 -padx 5;");
nodetk.eval("grid .c.status    -column 0 -row 6 -columnspan 2 -sticky we;");
nodetk.eval("grid columnconfigure .c  0 -weight 1; grid rowconfigure .c 5 -weight 1;");

nodetk.eval("bind $::lbox <<ListboxSelect>> \"showPopulation\";");
nodetk.eval("bind $::lbox <Double-1> \"sendGift\";");
nodetk.eval("bind . <Return> \"sendGift\";");

nodetk.createCommand('showPopulation', function() {
    var idx = nodetk.eval("$::lbox curselection");

    if (idx) {
        var code = nodetk.eval(`lindex $::countrycodes ${idx};`);
        var name = countries[idx].name;
        var population = countries[idx].population;
        nodetk.eval(`set ::statusmsg "The population of ${name} (${code}) is ${population}";`);
    }

    nodetk.eval("set ::sentmsg \"\";");
});

nodetk.createCommand('sendGift', function() {
    var idx = nodetk.eval("$::lbox curselection");
    if (idx) {
        nodetk.eval(`$::lbox see ${idx};`);
        var name = countries[idx].name;
        nodetk.eval(`set ::sentmsg "Sent $::gifts($::gift) to leader of ${name}";`);
    }


});

for (var i = 0; i < countries.length; i = i +2 ) {
    nodetk.eval(`$::lbox itemconfigure ${i} -background #f0f0ff;`);
}

nodetk.eval("set gift card;");
nodetk.eval("set sentmsg \"\";");
nodetk.eval("set statusmsg \"\";");
nodetk.eval("$::lbox selection set 0;");
nodetk.eval("showPopulation;");
