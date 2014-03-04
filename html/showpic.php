<?php

$files = glob("*.jpg");
$fileCount = count($files);
for ($i = ($fileCount-1); $i >= 0; $i--) {
    echo '<img src="'.$files[$i].'" alt="" width="450px">';
}

?>
