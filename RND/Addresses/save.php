<?php
	$cmd = './save.py "' . addslashes($_GET['name']) . '" "' . addslashes($_GET['surname']) . '" "' . addslashes($_GET['email']) . '" "' . addslashes($_GET['cell']) . '" "' . addslashes($_GET['street']) . '" "' . addslashes($_GET['zip']) . '" "' . addslashes($_GET['city']) . '"';
	$r = shell_exec(escapeshellcmd($cmd));
	echo "$r";
?>

