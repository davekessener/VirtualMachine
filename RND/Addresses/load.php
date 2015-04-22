<?php
	$r = shell_exec("./load.py " . addslashes($_GET["name"]) . " " . addslashes($_GET["surname"]));
	echo "$r";
?>

