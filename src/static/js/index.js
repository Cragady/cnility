async function testNativeClickHandler(e) {
  const res = await fetch('/api/test_native');
  const data = await res.json();
  if (res.status != 200) {
    console.error(data);
    return;
  }
  console.log(`data from server: `);
  console.log(data);
}

test_native_support?.addEventListener('click', testNativeClickHandler);
