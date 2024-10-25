// Toggle dark/light mode
document.getElementById('theme-toggle').addEventListener('click', () => {
    document.body.classList.toggle('dark-mode');
});

// Language change handler (mock function)
document.getElementById('language').addEventListener('change', (event) => {
    const lang = event.target.value;
    alert(`Language changed to: ${lang === 'en' ? 'English' : 'हिन्दी'}`);
    // Add functionality to actually change language content here
});
