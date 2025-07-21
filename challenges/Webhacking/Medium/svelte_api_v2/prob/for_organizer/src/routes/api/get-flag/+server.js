import { json } from '@sveltejs/kit';
import { flag, key2 } from '$env/static/private';

const flagPassword = 'hack';

function isAllowed(pass) {
	if (pass.toUpperCase() !== flagPassword.toUpperCase()) {
		return true;
	}
}

export async function GET({ url, getClientAddress }) {
	const u = url.searchParams.get('pass');
	const user_key2 = url.searchParams.get('key2');
	const ip = getClientAddress();

	if (!['localhost', '127.0.0.1', '', '::1'].includes(ip)) {
		return json({
			text: 'only localhost allowed',
			ip: ip
		});
	}

	if (!u) {
		return json({
			text: '?pass='
		});
	}

	if (!isAllowed(u)) {
		return json({
			text: 'not allowed'
		});
	}

	const a = u.toLowerCase();

	if (a === flagPassword && user_key2 === key2) {
		return json({
			text: flag
		});
	}

	return json({
		text: a
	});
}
