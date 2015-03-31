<?php
	shell_exec("./volume.py \"" . addslashes($_GET["volume"]) . '"');
	echo "./volume.py \"" . addslashes($_GET["volume"]) . '"';
?>

