param(
    [Parameter(Mandatory = $true)]
    [string]$PortName,

    [int]$BaudRate = 115200,

    [string]$CsvPath = "telemetry_$(Get-Date -Format 'yyyyMMdd_HHmmss').csv",

    [int]$DurationSeconds = 0
)

$ErrorActionPreference = 'Stop'

$fields = @(
    'POT', 'POT_mV', 'THR%', 'RUN', 'STATE', 'RPM', 'DIR', 'FLT',
    'VBUS_mV', 'IQ_mA', 'ID_mA', 'IQR_mA', 'VQ_mV', 'VMAX_mV', 'SAT'
)

$port = [System.IO.Ports.SerialPort]::new(
    $PortName,
    $BaudRate,
    [System.IO.Ports.Parity]::None,
    8,
    [System.IO.Ports.StopBits]::One
)
$port.ReadTimeout = 500

$writer = [System.IO.StreamWriter]::new(
    $CsvPath,
    $false,
    [System.Text.UTF8Encoding]::new($false)
)
$writer.AutoFlush = $true

try {
    $port.Open()
    $writer.WriteLine((@('Timestamp') + $fields) -join ',')

    $buffer = ''
    $started = Get-Date
    Write-Host "Logging $PortName at $BaudRate baud. Press Ctrl+C to stop."

    while (($DurationSeconds -eq 0) -or
           (((Get-Date) - $started).TotalSeconds -lt $DurationSeconds)) {
        if ($port.BytesToRead -gt 0) {
            $bytes = New-Object byte[] $port.BytesToRead
            $count = $port.Read($bytes, 0, $bytes.Length)
            $buffer += [System.Text.Encoding]::ASCII.GetString($bytes, 0, $count)

            while (($newline = $buffer.IndexOf("`n")) -ge 0) {
                $line = $buffer.Substring(0, $newline).TrimEnd("`r")
                $buffer = $buffer.Substring($newline + 1)

                if (-not $line.StartsWith('POT:')) {
                    continue
                }

                $values = @{}
                foreach ($token in $line.Split(' ')) {
                    if ($token.Contains(':')) {
                        $parts = $token.Split(':', 2)
                        $values[$parts[0]] = $parts[1]
                    }
                }

                $row = @((Get-Date).ToString('o'))
                foreach ($field in $fields) {
                    if ($values.ContainsKey($field)) {
                        $row += $values[$field]
                    }
                    else {
                        $row += ''
                    }
                }

                $writer.WriteLine(($row -join ','))
                Write-Host $line
            }
        }
        else {
            Start-Sleep -Milliseconds 10
        }
    }
}
finally {
    if ($port.IsOpen) {
        $port.Close()
    }
    $writer.Dispose()
    Write-Host "Telemetry saved to $CsvPath"
}
