<?php
	shell_exec('./add_track.py "' . addslashes($_GET["name"]) . '" "' . addslashes($_GET["url"]) . '"');
?>

