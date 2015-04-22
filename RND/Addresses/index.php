<html>
	<head>
		<title>Adressbuch</title>
		<script>
			function sendAJAX(file, handler, errhandler)
			{
				var xmlhttp;

				if(window.XMLHttpRequest)
				{
					xmlhttp = new XMLHttpRequest();
				}
				else
				{
					xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
				}

				xmlhttp.onreadystatechange = function()
				{
					if(xmlhttp.readyState == 4)
					{
						if(xmlhttp.status == 200)
						{
							if(typeof handler !== 'undefined') handler(xmlhttp.responseText.trim());
						}
						else
						{
							if(typeof errhandle !== 'undefined') errhandler();
						}
					}
				}

				xmlhttp.open("GET", file, true);
				xmlhttp.send();
			}
			function load_entry()
			{
				var name = document.getElementById("input_name");
				var surname = document.getElementById("input_surname");
				sendAJAX("load.php?name=" + escape(name.value) + "&surname=" + escape(surname.value), function(text)
				{
					set_content(text.split('\n'));
				});
			}
			function set_content(entries)
			{
				document.getElementById("input_name").value = entries[0];
				document.getElementById("input_surname").value = entries[1];
				document.getElementById("input_email").value = entries[2];
				document.getElementById("input_cell").value = entries[3];
				document.getElementById("input_street").value = entries[4];
				document.getElementById("input_zip").value = entries[5];
				document.getElementById("input_city").value = entries[6];
			}
			function save_entry()
			{
				var name = escape(document.getElementById("input_name").value);
				var surname = escape(document.getElementById("input_surname").value);
				var email = escape(document.getElementById("input_email").value);
				var cell = escape(document.getElementById("input_cell").value);
				var street = escape(document.getElementById("input_street").value);
				var zip = escape(document.getElementById("input_zip").value);
				var city = escape(document.getElementById("input_city").value);

				set_wait();

				sendAJAX("save.php?name=" + name + "&surname=" + surname + "&email=" + email + "&cell=" + cell + "&street=" + street + "&zip=" + zip + "&city=" + city, function(text)
				{
					set_info(text);
					setTimeout(clear_info, 2000);
				});
			}
			function clear_info()
			{
				set_info("");
			}
			function set_wait()
			{
				set_info("<i>...</i>");
			}
			function set_info(text)
			{
				document.getElementById("info_banner").innerHTML = text;
			}
			function initialize_contacts()
			{
<?php
			if(!empty($_GET))
			{
				echo 'document.getElementById("input_name").value = "' . $_GET["name"] . '";';
				echo 'document.getElementById("input_surname").value = "' . $_GET["surname"] . '";';
				echo 'load_entry();';
			}
?>
			}
			function create_link()
			{
				var name = document.getElementById("input_name");
				var surname = document.getElementById("input_surname");

				var link = "http://daveserver.no-ip.biz/kontakte/?name=" + escape(name.value) + "&surname=" + escape(surname.value);
//				var link = "http://localhost/adressbuch/?name=" + escape(name.value) + "&surname=" + escape(surname.value);
				set_info("<a href=\"" + link + "\">" + link + "</a>");
			}
			function export_db()
			{
				sendAJAX("export.php", function(text)
				{
					window.open(text, '_blank');
				});
			}
		</script>
	</head>
	<body onload="initialize_contacts();">
		<table>
<?php
			if(!empty($_GET))
			{
				echo '<tr><td>Vorname:</td><td><input type="text" id="input_name" value="' . $_GET['name'] . '" readonly /></td></tr>';
				echo '<tr><td>Nachname:</td><td><input type="text" id="input_surname" value="' . $_GET['surname'] . '" readonly /></td></tr>';
			}
			else if($_SERVER["REMOTE_ADDR"] === "127.0.0.1" || startsWith($_SERVER["REMOTE_ADDR"], "192.168"))
			{
				echo '<tr><td>Vorname:</td><td><input type="text" id="input_name" /></td></tr>';
				echo '<tr><td>Nachname:</td><td><input type="text" id="input_surname" /></td></tr>';
			}
			else
			{
				echo '<tr><td>Vorname:</td><td><input type="text" id="input_name" disabled /></td></tr>';
				echo '<tr><td>Nachname:</td><td><input type="text" id="input_surname" disabled /></td></tr>';
			}
?>
			<tr><td>E-Mail:</td><td><input type="text" id="input_email" /></td></tr>
			<tr><td>Handy-Nr:</td><td><input type="text" id="input_cell" /></td></tr>
			<tr><td>Strasse:</td><td><input type="text" id="input_street" /></td></tr>
			<tr><td>PLZ:</td><td><input type="text" id="input_zip" /></td></tr>
			<tr><td>Stadt:</td><td><input type="text" id="input_city" /></td></tr>
		</table>
		<input type="submit" id="btn_save" value="Speichern" onclick="save_entry();" />
<?php
		function startsWith($s, $p)
		{
			return (substr($s, 0, strlen($p)) === $p);
		}

		if($_SERVER["REMOTE_ADDR"] === "127.0.0.1" || startsWith($_SERVER["REMOTE_ADDR"], "192.168"))
		{
			echo '<input type="submit" id="btn_load" value="Laden" onclick="load_entry();" />';
			echo '<input type="submit" value="Link" onclick="create_link();" />';
			echo '<input type="submit" value="Export" onclick="export_db();" />';
		}
?>
		<br />
		<p id="info_banner"></p>
	</body>
</html>

