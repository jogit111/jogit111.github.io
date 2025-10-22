const express = require('express');
const axios = require('axios');
const app = express();
const cors = require('cors');

app.use(cors());
app.use(express.json());

const GITHUB_TOKEN = 'YOUR_GITHUB_TOKEN';
const GITHUB_USERNAME = 'YOUR_GITHUB_USERNAME';
const GITHUB_REPO = 'YOUR_REPO_NAME';

app.post('/save-rgb', async (req, res) => {
    const { rgb } = req.body;
    const filePath = 'rgb_value.txt';
    const message = 'Update RGB value';
    const url = `https://api.github.com/repos/${GITHUB_USERNAME}/${GITHUB_REPO}/contents/${filePath}`;

    try {
        // Get current file SHA (if it exists)
        let sha = null;
        const response = await axios.get(url, {
            headers: {
                'Authorization': `token ${GITHUB_TOKEN}`,
                'Accept': 'application/vnd.github.v3+json'
            }
        });
        if (response.data.sha) {
            sha = response.data.sha;
        }
    } catch (e) {
        // File doesn't exist, proceed without SHA
    }

    // Encode content
    const content = Buffer.from(rgb).toString('base64');

    // Create or update file
    const payload = {
        message,
        content,
        sha
    };

    try {
        await axios.put(url, payload, {
            headers: {
                'Authorization': `token ${GITHUB_TOKEN}`,
                'Accept': 'application/vnd.github.v3+json'
            }
        });
        res.json({ success: true });
    } catch (e) {
        res.status(500).json({ success: false, error: e.message });
    }
});

app.listen(3000, () => console.log('Server running on port 3000'));
