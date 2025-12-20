#!/usr/bin/env python3
import os
import sys
import json
import uuid
import html
import urllib.request
import urllib.error

# --- CONFIGURATION ---
# Ensure your C++ server passes this env var, or hardcode it for testing only.
API_KEY = "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"
MODEL = "openai/gpt-oss-120b"

# --- SILENCE STDERR ---
sys.stderr = open(os.devnull, 'w')

def get_session_id():
    cookie_string = os.environ.get('HTTP_COOKIE', '')
    cookies = {}
    if cookie_string:
        for cookie in cookie_string.split(';'):
            if '=' in cookie:
                key, value = cookie.strip().split('=', 1)
                cookies[key] = value
    if 'session_id' in cookies:
        return cookies['session_id']
    return str(uuid.uuid4())

def get_history(session_id):
    filepath = os.path.join('/tmp', f"session_{session_id}.json")
    if os.path.exists(filepath):
        try:
            with open(filepath, 'r') as f: return json.load(f)
        except: return []
    return []

def save_history(session_id, history):
    filepath = os.path.join('/tmp', f"session_{session_id}.json")
    try:
        with open(filepath, 'w') as f: json.dump(history, f)
    except: pass

def get_post_data():
    try: content_length = int(os.environ.get('CONTENT_LENGTH', 0))
    except: content_length = 0
    if content_length > 0:
        try:
            post_body = sys.stdin.read(content_length)
            import urllib.parse
            for pair in post_body.split('&'):
                if '=' in pair:
                    key, value = pair.split('=', 1)
                    if key == 'prompt':
                        return urllib.parse.unquote_plus(value)
        except: return None
    return None

def call_ai_api(history):
    """
    Calls Groq API (OpenAI Compatible)
    """
    url = "https://api.groq.com/openai/v1/chat/completions"
    
    api_messages = []
    api_messages.append({"role": "system", "content": "You are a helpful coding assistant. Keep answers short and concise. IMPORTANT: Your response must never exceed 500 words EVEN IF YOU'RE ASKED TO EXCEED IT."})
    
    for msg in history:
        role = "user" if msg['role'] == 'user' else "assistant"
        content = msg['parts'][0]['text']
        api_messages.append({"role": role, "content": content})

    payload = {
        "model": MODEL,
        "messages": api_messages,
        "max_tokens": 1000,
        "temperature": 0.7
    }
    
    # --- ADDED USER-AGENT TO HEADERS TO FIX 403 ERROR ---
    headers = {
        'Content-Type': 'application/json',
        'Authorization': f'Bearer {API_KEY}',
        'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36'
    }
    
    try:
        req = urllib.request.Request(url, data=json.dumps(payload).encode('utf-8'), headers=headers)
        with urllib.request.urlopen(req, timeout=10) as response:
            result = json.load(response)
            if 'choices' in result and len(result['choices']) > 0:
                return result['choices'][0]['message']['content'].strip()
            return "No response."
            
    except urllib.error.HTTPError as e:
        try:
            error_body = e.read().decode()
            return f"API Error {e.code}: {error_body}"
        except:
            return f"API Error {e.code}"
    except urllib.error.URLError as e:
        return f"Connection Error: {e.reason}"
    except Exception as e:
        return f"Script Error: {str(e)}"

# --- MAIN LOGIC ---
try:
    session_id = get_session_id()
    history = get_history(session_id)
    user_input = get_post_data()

    if user_input:
        history.append({'role': 'user', 'parts': [{'text': user_input}]})
        ai_response_text = call_ai_api(history)
        history.append({'role': 'model', 'parts': [{'text': ai_response_text}]})
        save_history(session_id, history)

    # OUTPUT HTML
    history_html = ""
    for msg in history:
        role_class = "user-msg" if msg['role'] == 'user' else "ai-msg"
        safe_text = html.escape(msg['parts'][0].get('text', '')).replace('\n', '<br>')
        history_html += f'<div class="message {role_class}">{safe_text}</div>'

    html_template = """
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>AI Chat</title>
    <style>
        body { margin: 0; font-family: sans-serif; background-color: #0d0d2b; color: white; height: 100vh; display: flex; flex-direction: column; overflow: hidden; }
        .background { position: fixed; top: 0; left: 0; width: 100%; height: 100%; z-index: -1; }
        .cube { position: absolute; width: 60px; height: 60px; border: 2px solid rgba(50, 100, 255, 0.3); background: rgba(50, 100, 255, 0.1); animation: move 10s linear infinite; }
        .cube:nth-child(1) { top: 10%; left: 10%; animation-duration: 12s; }
        .cube:nth-child(2) { top: 70%; left: 80%; width: 100px; height: 100px; animation-duration: 20s; animation-direction: reverse; }
        .cube:nth-child(3) { top: 40%; left: 40%; width: 40px; height: 40px; animation-duration: 15s; }
        @keyframes move { 0% { transform: translateY(0) rotate(0deg); opacity: 1; } 100% { transform: translateY(-800px) rotate(720deg); opacity: 0; } }
        
        .chat-container { flex: 1; overflow-y: auto; padding: 20px; display: flex; flex-direction: column; gap: 15px; max-width: 800px; margin: 0 auto; width: 100%; }
        .message { padding: 15px; border-radius: 15px; max-width: 70%; line-height: 1.5; word-wrap: break-word; }
        .user-msg { align-self: flex-end; background-color: #2b55d6; color: white; border-bottom-right-radius: 2px; }
        .ai-msg { align-self: flex-start; background-color: #e67e22; color: white; border-bottom-left-radius: 2px; }
        
        .input-area { background: rgba(0, 0, 0, 0.3); padding: 20px; display: flex; justify-content: center; }
        form { display: flex; width: 100%; max-width: 800px; gap: 10px; }
        input[type="text"] { flex: 1; padding: 15px; border-radius: 25px; border: none; background: rgba(255, 255, 255, 0.1); color: white; outline: none; font-size: 16px; }
        button { padding: 10px 25px; border-radius: 25px; border: none; background: #2b55d6; color: white; cursor: pointer; font-weight: bold; }
        button:hover { background: #1a3a9c; }
    </style>
</head>
<body>
    <div class="background"><div class="cube"></div><div class="cube"></div><div class="cube"></div></div>
    <div class="chat-container" id="chatBox">
        {chat_history_html}
    </div>
    <div class="input-area">
        <form action="" method="POST" id="chatForm">
            <input type="text" name="prompt" placeholder="Type a message..." required autocomplete="off" autofocus>
            <button type="submit">Send</button>
        </form>
    </div>
    <script>
        const chatBox = document.getElementById('chatBox');
        chatBox.scrollTop = chatBox.scrollHeight;
        document.getElementById('chatForm').addEventListener('submit', function() {
            chatBox.scrollTop = chatBox.scrollHeight;
            document.querySelector('input').blur();
        });
    </script>
</body>
</html>
"""
    
    print(f"Set-Cookie: session_id={session_id}; Max-Age=86400; Path=/")
    print(html_template.replace('{chat_history_html}', history_html))

except Exception:
    sys.exit(1)