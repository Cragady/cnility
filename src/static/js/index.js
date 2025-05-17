async function basicFetch(path) {
  const res = await fetch(path);
  const data = await res.json();
  if (res.status != 200) {
    console.error(data);
    return;
  }
  console.log(`data from server: `);
  console.log(data);
}

async function basicFetchClickHandler(e) {
  const targetPath = e?.target?.dataset?.fetchPath || '';
  if (targetPath === '') {
    console.error("No data fetch path for element!");
    return;
  }
  await basicFetch(targetPath);
}

test_native_support?.addEventListener('click', basicFetchClickHandler);
convert_kandr_chars?.addEventListener('click', basicFetchClickHandler);
