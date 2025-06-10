# Run with the 'bypass' execution policy:
# powershell -executionpolicy bypass -File query_idn.ps1
$device_port = Get-PnpDevice -Class Ports | Where-Object {$_.Status -eq "OK" -and $_.InstanceId -match "^FTDI.*"} | ForEach-Object { $null = $_.FriendlyName -match ".*(COM\d+)" ; $matches[1] }

$port= new-Object System.IO.Ports.SerialPort $device_port,115200,None,8,one
$port.Open()
$port.WriteLine("*IDN?")
$port.ReadLine()
$port.Close()
