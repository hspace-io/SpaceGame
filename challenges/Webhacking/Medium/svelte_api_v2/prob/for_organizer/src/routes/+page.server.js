import { error } from '@sveltejs/kit';

/** @type {import('./$types').PageServerLoad} */
export async function load({ cookies }) {
	const uuid = crypto.randomUUID();

	// const req = fetch('http://localhost:5173/what-is-my-user-agent');
	// const res = await req;
	// const text = await res.text();

	const result = cookies.get('result');

	if (uuid) {
		return { uuid: uuid, result: result };
	}

	throw error(404, 'Not found');
}

export const actions = {
	default: async (event) => {
		//console.log(event);

		const data = await event.request.formData();
		const url = data.get('url');

		const req = fetch(url);
		const res = await req;
		const text = await res.text();

		event.cookies.set('result', text);

		return { success: true };
	}
};
