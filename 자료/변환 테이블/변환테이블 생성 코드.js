var sorted = Array(0x100);
sorted = sorted.fill([0x00,HID_NONE]);
var extend = Array(0x100);
extend = extend.fill([0x00,HID_NONE]);

for(var i=0;itable.length;i++) {
    var str = table[i][PS2 KEYCODE].split(',');

    if(str.length == 1) {
        sorted[parseInt(str[0])] = [table[i][HID],table[i][HID NAME]];
    } else if(str.length == 2){
        extend[parseInt(str[1])] = [table[i][HID],table[i][HID NAME]];
    }
}

document.write(Defines pre);
for(var i=0;itable.length;i++){
    document.write(#definet + table[i][HID NAME] + t + table[i][HID] + ;n);
}
document.write(pre);

document.write(Single Keycode pre);
for(var i=0;isorted.length;i++){
    document.write(sorted[i][1]+(i%5!=4,,n));
}
document.write(pre);

document.write(Extends Keycodepre);
for(var i=0;iextend.length;i++){
    document.write(extend[i][1]+(i%5!=4,,n));
}
document.write(pre);
