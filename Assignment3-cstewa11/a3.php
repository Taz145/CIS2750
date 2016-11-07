<?php
if (isset($_FILES["myFile"])) {
    move_uploaded_file($_FILES["myFile"]["tmp_name"], "./files/".$_FILES["myFile"]["name"]);
    chmod("./files/".$_FILES["myFile"]["name"], 0777);
}

if (isset($_POST['mOk'])) {
    $fileToConvert = "files/".$_POST['fConvert'];
    exec("./a3 ".$fileToConvert);
}

$dir = "./files/";
if($dh = opendir($dir)) {
    while ($file = readdir($dh)) {
        if ($file[0] != '.') {
            $file_info = pathinfo($file);
            if($file_info['extension'] != "info") {
                if($file_info['extension'] != "html") {
                    $files[] = $file;
                } else {
                    $htmlFiles[] = $file;
                } 
            }
        }
    }
    closedir($dh);
}

echo '
<!DOCTYPE HTML>
<HTML>
<title> cis2750 - A3 </title>
<head>
<style>
.modal {
    display: none;
    position: fixed;
    z-index: 1;
    left: 0;
    top: 0;
    width: 100%;
    height: 100%;
    background-color: rgb(0,0,0);
}

.modal-content {
    background-color: #fefefe;
    margin: 15% auto;
    padding: 20px;
}
</style>
</head>
<BODY>
    <form action="a3.php" method="post" enctype="multipart/form-data" style="display:inline-block";>
        <input type="file" name="myFile" value="Upload" onchange="form.submit()"/>
    </form>

<button id="convert" style="display:inline-block";>Convert</button>

<div id="modal" class="modal">
    <div class="modal-content">
        <p>Please select a file for conversion</p>
    <form action="a3.php" method="post">
        <select name="fConvert">';
        foreach($files as $file) {
            echo "<option>".$file."</option>";
        }
        echo'  </select>
            <br>
        <input type="submit" name="mOk" value="OK" style="displat:inline-block"; />
    </form>
        <button id="mClose" style="display:inline-block";>Close</button>
    </div>
</div>

<script>

var modal = document.getElementById("modal");
var open = document.getElementById("convert");
var mClose = document.getElementById("mClose");

open.onclick = function() {
    modal.style.display = "block";
}

mClose.onclick = function() {
    modal.style.display = "none";
}

</script>
<br>
<ul style="width:10%; float:left; list-style:none; padding-left:0">';
foreach ($files as $file) {
    echo "<li><a href=$dir$file>$file</a>"."</li>";
}
echo '
</ul>
<ul style="width:10%; float:left; list-style:none; padding-left:160px"> ';

foreach ($htmlFiles as $hf){
    echo "<li><a href=$dir$hf>$hf</a>"."</li>";
}

echo '
</ul>
</BODY>
</HTML> ';
?>
