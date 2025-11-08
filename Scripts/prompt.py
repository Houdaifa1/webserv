#!/usr/bin/env python3

import os
import cgi
import html
import json
import urllib.request
import sys

# --- Helper function to print the HTML structure ---
def print_html(content):
    """Prints the full HTML page with neon blue styling."""
    print(f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Gemini Interface</title>
    <style>
        body {{
            background-color: #000000;
            color: #00ccff;
            font-family: 'Courier New', Courier, monospace;
            padding: 2rem;
            max-width: 800px;
            margin: auto;
        }}
        h1 {{
            text-shadow: 0 0 5px #00ccff, 0 0 10px #00ccff;
            border-bottom: 2px solid #00ccff;
            padding-bottom: 5px;
        }}
        textarea, button {{
            background-color: #111;
            border: 1px solid #00ccff;
            color: #00ccff;
            padding: 0.75rem;
            border-radius: 4px;
            font-family: 'Courier New', Courier, monospace;
            width: 100%;
            box-sizing: border-box; /* Ensures padding doesn't affect width */
            margin-bottom: 1rem;
        }}
        button {{
            background-color: #00ccff;
            color: #000;
            cursor: pointer;
            font-weight: bold;
            width: auto;
        }}
        .response-box {{
            background-color: #0a0a0a;
            border: 1px solid #00ccff;
            padding: 1rem;
            border-radius: 8px;
            box-shadow: 0 0 10px #00ccff;
            white-space: pre-wrap;
            word-wrap: break-word;
            margin-top: 2rem;
        }}
        .error {{
            color: #ff3914; /* Neon Red */
            text-shadow: 0 0 5px #ff3914;
        }}
    </style>
</head>
<body>
    <h1>1337 Gemini Interface</h1>
    <p>Submit your query to the AI. (Max 50 words)</p>
    
    <form method="POST" action="prompt.py">
        <textarea name="prompt" rows="4" placeholder="Type your prompt here..."></textarea>
        <button type="submit">Generate Response</button>
    </form>
    
    {content}
</body>
</html>
""")

def get_gemini_response(api_key, user_prompt):
    """Calls the Gemini API and returns the text response."""
    
    api_url = f"https://generativelanguage.googleapis.com/v1beta/models/gemini-2.5-flash-preview-09-2025:generateContent?key={api_key}"
    
    # System prompt to enforce the output word limit
    system_prompt = "You are a helpful assistant. Your response must be 200 words or less. Be concise."
    
    payload = {
        "contents": [{"parts": [{"text": user_prompt}]}],
        "systemInstruction": {"parts": [{"text": system_prompt}]},
    }
    
    data = json.dumps(payload).encode('utf-8')
    req = urllib.request.Request(api_url, data=data, headers={'Content-Type': 'application/json'})
    
    try:
        with urllib.request.urlopen(req, timeout=10) as response:
            result = json.loads(response.read().decode('utf-8'))
            
            if "candidates" in result:
                text = result['candidates'][0]['content']['parts'][0]['text']
                return text
            else:
                return f"[API Error] Invalid response structure: {json.dumps(result)}"

    except urllib.error.HTTPError as e:
        return f"[API Error] HTTP {e.code}: {e.read().decode('utf-8')}"
    except Exception as e:
        return f"[CGI Error] Failed to connect to API: {str(e)}"

# --- Main script logic ---
def main():
    method = os.environ.get("REQUEST_METHOD", "GET")
    response_content = ""

    if method == "POST":
        form = cgi.FieldStorage()
        prompt = form.getvalue("prompt", "").strip()
        
        if not prompt:
            response_content = "<div class='response-box error'>[Error] Prompt cannot be empty.</div>"
            print_html(response_content)
            return

        # Check word count
        word_count = len(prompt.split())
        if word_count > 50:
            response_content = "<div class='response-box error'>Please respect the word limit. (Max 50 words)</div>"
            print_html(response_content)
            return

        # Get API Key from environment
        api_key = "AIzaSyCelae8AGlqUsdK7XDnvebTy8lEhaMuFPI"
        if not api_key:
            response_content = "<div class='response-box error'>[CGI Config Error] GEMINI_API_KEY is not set in the server environment.</div>"
            print_html(response_content)
            return

        # Call API
        api_response = get_gemini_response(api_key, prompt)
        escaped_response = html.escape(api_response)
        # Do the newline replacement *before* the f-string to avoid the SyntaxError
        # I'm also changing r'\\n' to '\n' which is what the API likely returns.
        formatted_response = escaped_response.replace('\n', '<br>')
        
        response_content = f"""
        <div class="response-box">
            <strong>Your Prompt:</strong>
            <p>{html.escape(prompt)}</p>
            <hr>
            <strong>Gemini's Response:</strong>
            <p>{formatted_response}</p>
        </div>
        """
        
    print_html(response_content)

if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        # Generic error handling if main() fails
        print("<!DOCTYPE html><html><head><title>CGI Error</title></head>")
        print("<body><h1>CGI Script Error</h1>")
        print(f"<pre>{html.escape(str(e))}</pre>")
        print("</body></html>")