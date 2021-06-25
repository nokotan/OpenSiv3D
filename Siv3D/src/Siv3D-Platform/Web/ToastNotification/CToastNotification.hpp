//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2019 Ryo Suzuki
//	Copyright (c) 2016-2019 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# pragma once
# include <Siv3D/ToastNotification.hpp>
# include <ToastNotification/IToastNotification.hpp>
# include <Siv3D/HashTable.hpp>

namespace s3d
{
	class CToastNotification : public ISiv3DToastNotification
	{
	public:

		struct Notification 
		{
		public:
			using IDType = int32;

			ToastNotificationState state = ToastNotificationState::None;
		};

		enum class NotificationPermission
		{
			Default,

			Granted,

			Denied
		};

	private:

		static void OnStateUpdate(Notification::IDType idx, ToastNotificationState state, void* userData);

		static void OnCreateNotification(Notification::IDType idx, void* userData);

		HashTable<Notification::IDType, Notification> m_notifications;

	public:

		CToastNotification();

		~CToastNotification() override;

		void init() override;

		bool isAvailable() const override;

		NotificationID show(const ToastNotificationProperty& prop) override;

		ToastNotificationState getState(NotificationID id) override;

		Optional<size_t> getAction(NotificationID id) override;

		void hide(NotificationID id) override;

		void clear() override;

		void onStateUpdate(size_t index, ToastNotificationState state, const Optional<int32>& option) override;
	};
}
