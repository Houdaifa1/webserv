#!/bin/bash
# sleep 11
# Use a "here document" to output the HTML structure
cat <<EOF
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>CGI Details</title>
    <style>
        body {
            background-color: #000;
            color: #0f0;
            font-family: 'Courier New', Courier, monospace;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
            flex-direction: column;
        }
        h1 {
            font-size: 5em;
            text-transform: uppercase;
            letter-spacing: 0.1em;
            text-shadow: 0 0 5px #0f0, 0 0 10px #0f0, 0 0 20px #0f0, 0 0 40px #0f0;
        }
        .info-box {
            border: 2px solid #0f0;
            padding: 2em;
            margin-top: 2em;
            box-shadow: 0 0 15px #0f0, inset 0 0 15px #0f0;
            border-radius: 10px;
        }
        .info-box p {
            margin: 0.5em 0;
            font-size: 1.2em;
        }
        .info-box .label {
            font-weight: bold;
        }
    </style>
</head>
<body>
    <h1>BASH</h1>
    <div class="info-box">
        <p><span class="label">Current Time (UTC):</span> $(date -u +"%Y-%m-%d %H:%M:%S")</p>
        <p><span class="label">Request Method:</span> $REQUEST_METHOD</p>
        <p><span class="label">Client IP Address:</span> $REMOTE_ADDR</p>
        <p><span class="label">Client Port:</span> $REMOTE_PORT</p>
        <p><span class="label">Server Protocol:</span> $SERVER_PROTOCOL</p>
        <p><span class="label">User Agent:</span> $HTTP_USER_AGENT</p>
    </div>
</body>
</html>
EOF

# sleep 20