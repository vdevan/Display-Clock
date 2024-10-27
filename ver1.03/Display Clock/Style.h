char Style[] PROGMEM  = R"rawliteral(
<style>
body {background-color: #b4e0b4;}
h4 {margin: 2px;}
h1 {color:blue;text-align:center;}
h2 {color: blue;text-align: center;padding-top:15px;}
.container {margin: auto;width: 90%;min-width: 440px;background-color: #b5eafe;padding: 0px 10px;}
@media(min-width:1200px) {.container {margin: auto;width: 35%;}}
@media(min-width:768px) and (max-width:1200px) {
.container {margin: auto;width: 35%;}}
.q {float: right;width: 64px;text-align: right;}
.l {background: url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==') no-repeat left center;
background-size: 1em;}
.scnnet {background-color: beige;padding: 5px;margin-bottom: 5px;}
.strnet {background-color: bisque;padding: 5px;}
.inf {padding: 2px;}
textarea {white-space: pre;}
table {width: 95%;margin: auto;border-collapse: collapse;}
p {margin-block: 0.8em;}
td {padding: 2px;}
td.stbtn {border: 1px solid #808080;padding: 2px 10px;width: 90%;}
.ti {width:95%;}
.btn {border-radius: 4px;border: 0;color: red;cursor: pointer;display: inline-block;margin: 2px;padding: 5px;position: relative;background-color: beige;box-shadow: 0px 6px 5px -4px rgba(0,0,0,0.52);border: 0;}
.btn:hover {background: #b4e0b4;}
.btn:active, #btn:focus {background: #b5eafe;}
.btn:disabled {background-color: rgba(100,100,100,0.1);cursor: not-allowed;}
.txt {padding: 5px;}
.idtxt {width: 40px;padding: 5px;}
.attn {color: red;font-weight: 600;}
.log {outline: none;height:100px; column-width:12em;background-color:azure;padding:10px;border:inset;color:darkolivegreen;margin-bottom:20px;margin-top:10px;width:95%;}
tr.dp {vertical-align: middle;text-align: center;}
.scrl {margin: 2px;padding: 5px;width: 90%;}
.scrlb {margin: 2px;padding: 5px;width: 90%;border-style: dashed;}
td.hd{padding:10px;}
tr.ti{height:60px;}
.inf{color:#9f0000;font-weight:500;}
.blk{padding: 5px;width: 160px;border-style: dotted;}
.tab {overflow: hidden;border: 1px solid #ccc;background-color: #e7e9b2;}
.tab button {background-color: inherit;float: left;border: none;outline: none;cursor: pointer;padding: 14px 16px;transition: 0.3s;}
.tab button:hover {background-color: #ddd;}
.tab button.active {background-color: #7ccf98;}
.tabcontent {display: none;padding: 6px 12px;border: 1px solid #ccc;border-top: none;}
</style>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="shortcut icon" href='data:image/x-icon;base64,Qk1+AAAAAAAAAD4AAAAoAAAAEAAAABAAAAABAAEAAAAAAEAAAADDDgAAww4AAAAAAAAAAAAAAAAAAP///wD//wAA//0AAOAHAADAAwAAwAMAAMADAADAAwAAwMMAAMPjAADGAwAAzAMAAMADAADAAwAA4AcAAP//AAD//wAA' />
)rawliteral";
//style and link rel for favicon is common to both Captive and Web page 