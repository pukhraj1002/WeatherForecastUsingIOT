// Toggle dark/light mode
document.getElementById('theme-toggle').addEventListener('click', function() {
    document.body.classList.toggle('dark-mode');
});

// Handle language changes (placeholder, not functional)
document.getElementById('language').addEventListener('change', function() {
    let lang = this.value;
    alert('Language changed to: ' + (lang === 'en' ? 'English' : 'हिन्दी'));
    // Implement actual language change functionality here
});
