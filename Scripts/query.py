#!/usr/bin/env python3

import os
import cgi
import html
import urllib.parse

# --- Helper function to print the HTML structure ---
def print_html(title, content):
    """Prints the full HTML page with neon blue styling."""
    
    print(f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>{title}</title>
    <style>
        body {{
            background-color: #000000;
            color: #00ccff;
            font-family: 'Courier New', Courier, monospace;
            padding: 2rem;
            max-width: 800px;
            margin: auto;
        }}
        h1, h2 {{
            text-shadow: 0 0 5px #00ccff, 0 0 10px #00ccff;
            border-bottom: 2px solid #00ccff;
            padding-bottom: 5px;
        }}
        pre {{
            background-color: #0a0a0a;
            border: 1px solid #00ccff;
            padding: 1rem;
            border-radius: 8px;
            box-shadow: 0 0 10px #00ccff;
            white-space: pre-wrap;
            word-wrap: break-word;
        }}
        ul {{
            list-style-type: none;
            padding-left: 0;
        }}
        li {{
            background-color: #0a0a0a;
            border: 1px solid #00ccff;
            padding: 0.75rem;
            margin-bottom: 0.5rem;
            border-radius: 4px;
        }}
        .key {{
            color: #ffffff;
            font-weight: bold;
        }}
        .value {{
            color: #00ccff;
        }}
    </style>
</head>
<body>
    <h1>Query String Parser</h1>
    {content}
</body>
</html>
""")

# --- Main script logic ---
def main():
    method = os.environ.get("REQUEST_METHOD", "GET")
    content = ""

    if method == "POST":
        # --- POST RESPONSE ---
        # Parse form data just to show it
        form = cgi.FieldStorage()
        
        content = "<h2>POST Data Received</h2>"
        if not form.keys():
            content += "<p>No POST data was sent in the request body.</p>"
        else:
            content += "<ul>"
            for key in form.keys():
                # html.escape to prevent XSS
                escaped_key = html.escape(key)
                escaped_value = html.escape(form.getvalue(key))
                content += f"<li><span class='key'>{escaped_key}:</span> <span class='value'>{escaped_value}</span></li>"
            content += "</ul>"

    else:
        # --- GET RESPONSE ---
        query_string = os.environ.get("QUERY_STRING", "")
        
        content = "<h2>GET Query String Parameters</h2>"
        
        if not query_string:
            content += "<p>No query string was provided in the URL.</p>"
        else:
            # Parse the query string into a list of key-value pairs
            # urllib.parse.parse_qsl correctly handles URL decoding
            query_list = urllib.parse.parse_qsl(query_string)
            
            content += "<ul>"
            for key, value in query_list:
                # html.escape to prevent XSS
                escaped_key = html.escape(key)
                escaped_value = html.escape(value)
                content += f"<li><span class='key'>{escaped_key}:</span> <span class='value'>{escaped_value}</span></li>"
            content += "</ul>"
            
            content += "<h2>Raw Query String</h2>"
            content += f"<pre>{html.escape(query_string)}</pre>"

    print_html("Query Parser", content)

if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        # Generic error handling
        print("<!DOCTYPE html><html><head><title>CGI Error</title></head>")
        print("<body><h1>CGI Script Error</h1>")
        print(f"<pre>{html.escape(str(e))}</pre>")
        print("</body></html>")